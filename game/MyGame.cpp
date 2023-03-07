#include "stdafx.h"
#include "MyGame.h"

CMyGame::CMyGame(void) : 
	startScreen(400,300,"startScreen.bmp",0)
	// to initialise more sprites here use a comma-separated list
{
	// TODO: add initialisation here
}

CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	// do not run game logic when in menu mode
	if (IsMenuMode()) return;

	Uint32 t = GetTime();

	// TODO: add the game update code here
	player.Update(t);
    
	if (player.GetY() < 0) GameOver();
	PlayerControl();
}


void CMyGame::OnDraw(CGraphics* g)
{
	// draw menu when in menu mode
	if (IsMenuMode())
	{
		startScreen.Draw(g);
		return;
	}
	// ----- scrolling -------------------------------
    // game world (background image) is of size 2400x600
	int leftScreenLimit = 300;
	int rightScreenLimit = 1900; // 2400-800+300
	
	// do not scroll if left to the left limit
	if (player.GetX() < leftScreenLimit)
	{
		g->SetScrollPos(0, 0);  
	}
    // we scroll the whole game world according to the player position
	// unless we reached the left or right screen limits
	else if (player.GetX() >= leftScreenLimit && player.GetX() <= rightScreenLimit)
	{
		g->SetScrollPos(leftScreenLimit - player.GetX(), 0);
	}
	// we stop scrolling with the player once we have reached the right limit
	// and remain in that scrolling position at the right limit
	else if (player.GetX() > rightScreenLimit)
	{
		g->SetScrollPos(leftScreenLimit-rightScreenLimit, 0);
	}
	
	background.Draw(g);

	player.Draw(g);
	  
	 // don't scroll the overlay screen
	 g->SetScrollPos(0, 0);



	 *g << font(28) << color(CColor::Red()) << xy(10, 570) << "player status: " << player.GetStatus();
	 *g << font(28) << color(CColor::Green()) << xy(10, 545) << "player x-pos: " << (int)player.GetX();

	 if (IsGameOverMode())  *g << font(40) << color(CColor::Green()) << xy(400, 300) << "Game Over";
	
}

void CMyGame::PlayerControl()
{

	float hwalkSpeed = 225; // horizontal walking speed
	float vwalkSpeed = 100; // vertical walking speed

	float lowerlimit = 30;  // setting the limits
	float upperlimit = 200;
	float leftlimit = 30;
	float rightlimit = 2400-30;
	
	enum animation_state { NONE, STANDLEFT, STANDRIGHT, WALKLEFT, WALKRIGHT };
	/* 
	  player status variable is used to indicate which state the player is in

	  0: None
	  1: standing left looking
	  2: standing right looking
	  3: walking left
	  4: walking right
	*/

	// preserving the current status
    int old_animation_status = player.GetStatus();
    
	// resetting the player to standing each time and then
	// changing this according to state and keyboard input
    player.SetMotion(0, 0);
	if (player.GetStatus()==WALKLEFT) player.SetStatus(STANDLEFT);
	if (player.GetStatus()==WALKRIGHT) player.SetStatus(STANDRIGHT);
	
	// setting the speeds and animation status according to the keyboard controls
	if (IsKeyDown(SDLK_RIGHT)) { player.SetXVelocity(hwalkSpeed); player.SetStatus(WALKRIGHT);}
	if (IsKeyDown(SDLK_LEFT)) { player.SetXVelocity(-hwalkSpeed); player.SetStatus(WALKLEFT); }
	if (IsKeyDown(SDLK_UP))
	{
		if (player.GetStatus() == STANDLEFT) player.SetStatus(WALKLEFT);
		if (player.GetStatus() == STANDRIGHT) player.SetStatus(WALKRIGHT);
		player.SetYVelocity(vwalkSpeed);
	}
	if (IsKeyDown(SDLK_DOWN))
	{
		if (player.GetStatus() == STANDLEFT) player.SetStatus(WALKLEFT);
		if (player.GetStatus() == STANDRIGHT) player.SetStatus(WALKRIGHT);
		player.SetYVelocity(-vwalkSpeed);
	}

	// changing animation sequences if different from previous status
	if (player.GetStatus() != old_animation_status)
	{
		if (player.GetStatus() == WALKLEFT) player.SetAnimation("walkleft", 10);
		if (player.GetStatus() == WALKRIGHT) player.SetAnimation("walkright", 10);
		if (player.GetStatus() == STANDRIGHT) player.SetImage("standright");
		if (player.GetStatus() == STANDLEFT) player.SetImage("standleft");
	}

	// setting the player movement limits
	if (player.GetY() > upperlimit) player.SetY(upperlimit);
	if (player.GetY() < lowerlimit) player.SetY(lowerlimit);
	if (player.GetX() < leftlimit) player.SetX(leftlimit);
	if (player.GetX() > rightlimit) player.SetX(rightlimit);

	player.Update(GetTime());

}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{   
	// loading the game map
	background.LoadImage("background.bmp", "map1", CColor::Black());
	background.SetImage("map1"); background.SetPosition(1200, 300);

	// configuring the animations for the player sprite
	player.LoadImage("joe.bmp", "standright", 6, 1, 2, 0, CColor::Black());
	player.AddImage("joe.bmp", "walkright", 6, 1, 0, 0, 2, 0, CColor::Black());
	player.LoadImage("joe.bmp", "standleft", 6, 1, 5, 0, CColor::Black());
	player.AddImage("joe.bmp", "walkleft", 6, 1, 3, 0, 5, 0,CColor::Black());

	player.SetImage("standright");
	player.SetStatus(2);

}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{   

	SetupLevel1();
}


void CMyGame::SetupLevel1()
{



}

// setting up a level 
void CMyGame::SetupLevel2()
{
	

}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
    player.SetPosition(100, 50);
	player.SetMotion(0, 0);
	player.SetImage("standright");
	player.SetStatus(2);
	
}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{	

}

// called when the game is over
void CMyGame::OnGameOver()
{
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();

	
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
	// start game with a left mouse button click
	if (IsMenuMode()) StartGame();
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
