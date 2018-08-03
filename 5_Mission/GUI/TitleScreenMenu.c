/*! Xbox menu */
class TitleScreenMenu extends UIScriptedMenu
{
	protected DayZIntroScene m_scene;
	
	void TitleScreenMenu()
	{
	}
	
	void ~TitleScreenMenu()
	{
	}
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/xbox/day_z_title_screen.layout");
		
		MissionMainMenu mission = MissionMainMenu.Cast( g_Game.GetMission() );
		m_scene = mission.GetIntroScene();
		
		RichTextWidget text_widget = RichTextWidget.Cast( layoutRoot.FindAnyWidget("InputPromptText") );
		if (text_widget)
		{
			string gamertag;
			GetGame().GetPlayerName(gamertag);
#ifdef PLATFORM_XBOX
		text_widget.SetText("Press <image set=\"xbox_buttons\" name=\"A\" /> to start the game");
#endif
		
#ifdef PLATFORM_PS4
		text_widget.SetText("Press <image set=\"playstation_buttons\" name=\"cross\" /> to start the game");
#endif
		}
		return layoutRoot;
	}
	
	override void OnShow()
	{
		if( g_Game.GetGameState() != DayZGameState.CONNECTING )
			g_Game.SelectUser();
	}
	
	override void OnHide()
	{
		
	}
	
	override void Update(float timeslice)
	{
		if( GetGame().GetInput().GetActionDown( UAUISelect, false ) )
		{
			EnterScriptedMenu(MENU_MAIN);
		}
		#ifdef PLATFORM_WINDOWS
			
		#endif
	}
	
}