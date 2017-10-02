#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Make MyMesh object (array)
	m_pMesh = new MyMesh[60];

	for (int i = 0; i < 60; i++) //initialize all mymesh objects to a given size and color
	{
		m_pMesh[i].GenerateCube(1.0f, C_BLUE);
	}

	//Make MyMesh object
	//m_pMesh1 = new MyMesh();
	//m_pMesh1->GenerateCube(1.0f, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	static matrix4 m4Model;
	//vector3 v3Position(0, 0, 0); //what position we want
	vector3 v3PositionMove(.0005, .0005 * glm::cos(m4Model[3].x), 0); //move right at rate of .0005 per update, follow (co?)sine wave movement in y
	// was v3PositionScale in commented lines

	//static matrix4 m4Model; //making this static means it's the same model being used every time so it keeps being translated without resetting
	//remove static to keep from updating every single update

	//m4Model[3] = vector4(v3Position, 1); //set translation portion of matrix equal to position (1 as last value since it's a 4d matrix)

	/*
	m4Model[1][1] = .5f;
	m4Model[2][2] = .5f;  Scales down by .5
	m4Model[3][3] = .5f;
	*/

	//m4Model = glm::translate(IDENTITY_M4, v3Position);
	//m4Model = glm::translate(m4Model, v3Position); //m4Model starts as the identity matrix, keeps getting position added to it if static

	//m4Model = glm::scale(m4Model, v3PositionScale);

	//m4Model = glm::rotate(m4Model, 1.f, vector3(0, 0, 1));
	//vector specifiies axis of rotation, float specifies angle of rotation

	//m_pMesh[0].Render(m4Projection, m4View, m4Model);

	//build space invader, line by line horizontally
	for (int i = 0; i < 11; i++) 
	{
		//large middle line 11 across
		m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(m4Model, vector3(i, 0, 0)));
		if (i != 1 && i != 9)
		{
			m_pMesh[i + 11].Render(m4Projection, m4View, m4Model * glm::translate(m4Model, vector3(i, -1, 0)));
		}
		//line below that
		if(i == 0 || i == 2 || i == 8 || i == 10)
		{
			m_pMesh[i + 20].Render(m4Projection, m4View, m4Model * glm::translate(m4Model, vector3(i, -2, 0)));
		}
		//line below
		if(i == 3 || i == 4 || i == 6 || i == 7)
		{
			m_pMesh[i + 24].Render(m4Projection, m4View, m4Model * glm::translate(m4Model, vector3(i, -3, 0)));
		}
		//line above 11 across line
		if(i != 0 && i != 3 && i != 7 && i != 10)
		{
			m_pMesh[i + 28].Render(m4Projection, m4View, m4Model * glm::translate(m4Model, vector3(i, 1, 0)));
		}
		//line above that
		if(i != 0 && i != 1 && i != 9 && i != 10)
		{
			m_pMesh[i + 35].Render(m4Projection, m4View, m4Model * glm::translate(m4Model, vector3(i, 2, 0)));
		}
		//line above that (antena base)
		if (i == 3 || i == 7)
		{
			m_pMesh[i + 42].Render(m4Projection, m4View, m4Model * glm::translate(m4Model, vector3(i, 3, 0)));
		}
		//line above that (antena top)
		if(i == 2 || i == 8)
		{
			m_pMesh[i + 44].Render(m4Projection, m4View, m4Model * glm::translate(m4Model, vector3(i, 4, 0)));
		}
	}

	//v3PositionMove.y = 3 * glm::sin(1 / 2 * m4Model[3][0]);

	//move all cubes that make up the space invader in the same way
	for (int j = 0; j < 60; j++) {
		m4Model = glm::translate(m4Model, v3PositionMove);
	}

	//m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
	//m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3( 3.0f, 0.0f, 0.0f)));
		
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete[] m_pMesh;
		m_pMesh = nullptr;
	}
	//SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}