#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Jake Toporoff - jxt8029@rit.edu"

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
	}
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	m4Offset = glm::rotate(IDENTITY_M4, 90.0f, AXIS_Z);

	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 90.0f, AXIS_X));
		//Get a timer
		static float fTimer = 0;	//store the new timer
		static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
		fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

		static int wholeSeconds = 0; //number of entire seconds passed, used to check for stop updates
		static int current = 0; //current vertex
		static int next = 1; //next vertex

		float timer = fTimer - wholeSeconds; //timer for checking when the next whole second is reached

		//update all variables every second
		if (timer >= 1.f) {
			current++;
			next++;
			wholeSeconds++;
		}

		//find decimal portion of timer value
		float lerpVal = fTimer - (long)(fTimer);

		//find radius for every torus, first is .95 and every one after that is .5 greater
		float radius = .95f + (.5f * i);

		float incrementAngle = (2 * PI) / (i + 3); //angle to increment based on number of sides on the torus
		float angle1 = incrementAngle * current; //increment based on current vertex
		float angle2 = incrementAngle * next; //have next increment be the next vertex of the shape

		//calculate the current and next positions
		vector3 v3CurrentPos = vector3(cos(angle1) * radius, sin(angle1) * radius, 0);
		vector3 v3NextPos = vector3(cos(angle2) * radius, sin(angle2) * radius, 0);

		//lerp between current and next over 1 second
		vector3 spherePos = glm::lerp(v3CurrentPos, v3NextPos, lerpVal);

		matrix4 m4Model = glm::translate(m4Offset, spherePos);
		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);
	}

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
	//release GUI
	ShutdownGUI();
}