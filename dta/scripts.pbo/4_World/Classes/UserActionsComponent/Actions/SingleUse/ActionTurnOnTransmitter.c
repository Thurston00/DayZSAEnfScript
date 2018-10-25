class ActionTurnOnTransmitterCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CASingleUseTurnOnPlugged;
	}
}

class ActionTurnOnTransmitter: ActionTurnOnWhileInHands
{
	void ActionTurnOnTransmitter()
	{
		m_CallbackClass = ActionTurnOnTransmitterCB;
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_ON;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_ITEM_ON;
		m_MessageSuccess = "I have turned it on.";
		m_MessageStartFail = "Cannot turn on the device without a power source.";
		m_MessageFail = "It's broken.";
	}
	
	override int GetType()
	{
		return AT_TURN_ON_TRANSMITTER;
	}

	override string GetText()
	{
		return "#turn_on";
	}
		
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item.IsTransmitter() )
		{
			if ( item.HasEnergyManager() && !item.GetCompEM().IsWorking() )
			{
				if ( item.IsInherited( TransmitterBase ) )
				{
					TransmitterBase transmitter = TransmitterBase.Cast( item );
					
					transmitter.DisplayRadioInfo( "OFF", player );
				}
				
				return true;
			}
		}
		
		return false;
	}
		
	override void OnCompleteServer( ActionData action_data )
	{
		action_data.m_MainItem.GetCompEM().SwitchOn();
	}
}