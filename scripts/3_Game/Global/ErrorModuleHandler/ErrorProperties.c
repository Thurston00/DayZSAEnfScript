//! Class which holds the properties and handling of an error
class ErrorProperties
{
	const string EP_HEADER_FORMAT_STRING = "%1 (%2)"; //!< Formating for header (%1 = Header; %2 = ErrorCode)
	const string EP_MESSAGE_FORMAT_STRING = "%1\n(%2)"; //!< Formating for message (%1 = Message; %2 = AdditionalInfo)
	
	protected string m_Message; //!< Message which will appear on Client
	protected string m_ServerMessage; //!< Message which will appear on Server
	
	void ErrorProperties(string message, string serverMessage)
	{
		m_Message = message;
		m_ServerMessage = serverMessage;
	}
		
	void HandleError(int errorCode, string additionalInfo = "") {}
	
	string GetClientMessage(string additionalInfo = "")
	{
		if ( additionalInfo != "" )
			return string.Format(EP_MESSAGE_FORMAT_STRING, m_Message, additionalInfo);
		else
			return m_Message;
	}
	
	string GetServerMessage(string additionalInfo = "")
	{
		if ( additionalInfo != "" )
			return string.Format(EP_MESSAGE_FORMAT_STRING, m_ServerMessage, additionalInfo);
		else
			return m_ServerMessage;
	}
}

//! Error which shows a generic Dialogue UI
class DialogueErrorProperties : ErrorProperties
{
	protected string m_Header;
	protected int m_DialogButtonType;
	protected int m_DefaultButton;
	protected int m_DialogMeaningType;
	protected int m_MenuId;
	protected UIScriptedMenu m_Handler;
	protected bool m_DisplayAdditionalInfo;
	
	void DialogueErrorProperties(string message, string serverMessage, string header, int menuId = -1, UIScriptedMenu handler = null, int dialogButtonType = DBT_OK, int defaultButton = DBB_OK, int dialogMeaningType = DMT_EXCLAMATION, bool displayAdditionalInfo = true)
	{
		m_Header = header;
		m_DialogButtonType = dialogButtonType;
		m_DefaultButton = defaultButton;
		m_DialogMeaningType = dialogMeaningType;
		m_MenuId = menuId;
		m_Handler = handler;
		m_DisplayAdditionalInfo = displayAdditionalInfo;
	}
	
	override void HandleError(int errorCode, string additionalInfo = "")
	{
		if ( g_Game.IsMultiplayer() && g_Game.IsServer() )
			return;
		
		string message;
		if (m_DisplayAdditionalInfo && additionalInfo != "")
			message = string.Format(EP_MESSAGE_FORMAT_STRING, m_Message, additionalInfo);
		else
			message = m_Message;
		
		GetGame().GetUIManager().ShowDialog(
			string.Format(EP_HEADER_FORMAT_STRING, m_Header, ErrorModuleHandler.GetErrorHex(errorCode)),
			message, m_MenuId, m_DialogButtonType, m_DefaultButton, m_DialogMeaningType, m_Handler);
	}
	
	string GetHeader() { return m_Message; }
	int GetDialogButtonType() { return m_DialogButtonType; }
	int GetDefaultButton() { return m_DefaultButton; }
	int GetDialogMeaningType() { return m_DialogMeaningType; }
	int GetMenuId() { return m_MenuId; }
	UIScriptedMenu GetHandler() { return m_Handler; }
}