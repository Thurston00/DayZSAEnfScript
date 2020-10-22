class ActionPackTentCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_DEPLOY); //Use same time as Deploy
	}
	
	override void OnAnimationEvent(int pEventID)	
	{
		super.OnAnimationEvent( pEventID );
		
		switch (pEventID)
		{
			case UA_FINISHED:		

			break;
		}
	}
}

class ActionPackTent : ActionContinuousBase
{
	void ActionPackTent()
	{
		m_CallbackClass		= ActionPackTentCB;
		m_SpecialtyWeight 	= UASoftSkillsWeight.PRECISE_LOW;
		m_CommandUID		= 0;
		m_FullBody			= true;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTParent(10);
	}

	override string GetText()
	{
		return "#pack_tent";
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override bool IsUsingProxies()
	{
		return true;
	}

	override bool HasProgress()
	{
		return true;
	}
	
	override bool HasAlternativeInterrupt()
	{
		return true;
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{
		Object targetParent = action_data.m_Target.GetParent();
		TentBase tent = TentBase.Cast( targetParent );
		return tent.CanBePacked();
	}
	
	override ActionData CreateActionData()
	{
		PlaceObjectActionData action_data = new PlaceObjectActionData;
		return action_data;
	}
	
	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL)
	{	
		if ( super.SetupAction(player, target, item, action_data, extra_data ))
		{
			PlaceObjectActionData poActionData;
			poActionData = PlaceObjectActionData.Cast(action_data);
			poActionData.m_AlreadyPlaced = false;
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_2HD; //Call the animation
			return true;
		}
		return false;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		Object targetParent = target.GetParent();
		
		TentBase inventory_tent = TentBase.Cast(targetObject);
		if (inventory_tent && inventory_tent.GetHierarchyRootPlayer())
		{
			return false; //tent is somewhere in player's inventory
		}
		
		if ( player && targetObject && targetParent )
		{
			float max_action_distance = 1; //m_MaximalActionDistance;
			if ( targetParent.IsInherited(CarTent) ) 
			{
				max_action_distance = 5.0;
			}
			else if ( targetParent.IsInherited(LargeTent) ) 
			{
				max_action_distance = 4.0;
			}
			else if ( targetParent.IsInherited(MediumTent) )
			{
				max_action_distance = 3.0;
			}
			else if ( targetParent.IsInherited(PartyTent) )
			{
				max_action_distance = 5.0;
			}

			float distance = vector.DistanceSq(targetParent.GetPosition(),player.GetPosition());
			if (  distance <= max_action_distance * max_action_distance )	
			{
				TentBase tent = TentBase.Cast( targetParent );
				if ( tent.CanBePacked() )
				{
					array<string> selections = new array<string>;
					targetObject.GetActionComponentNameList(target.GetComponentIndex(), selections);
					
					for ( int s = 0; s < selections.Count(); s++ )
					{					
						if ( selections[s] == "pack" )
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	
	override void OnExecute( ActionData action_data )
	{
		Object targetParent = action_data.m_Target.GetParent();
		TentBase tent = TentBase.Cast( targetParent );
		tent.SetIsBeingPacked(true);
	}
	
	override void OnEnd( ActionData action_data )
	{
		Object targetParent = action_data.m_Target.GetParent();
		TentBase tent = TentBase.Cast( targetParent );
		tent.SetIsBeingPacked(false);
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{
		//vector offset = Vector(1, 0, 0);
		Object targetParent = action_data.m_Target.GetParent();
		TentBase tent = TentBase.Cast( targetParent );
		if (tent.CanBePacked())
		{
			tent.Pack( true );
			tent.SetPosition( action_data.m_Player.GetPosition() /*+ offset*/ ); //Add offset to allign tent with player
			tent.SetOrientation( action_data.m_Player.GetOrientation() );
			tent.PlaceOnSurface();
		}
	}
};