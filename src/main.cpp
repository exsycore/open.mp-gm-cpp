/*
 *  This Source Code Form is subject to the terms of the Mozilla Public License,
 *  v. 2.0. If a copy of the MPL was not distributed with this file, You can
 *  obtain one at http://mozilla.org/MPL/2.0/.
 *
 *  The original code is copyright (c) 2022, open.mp team and contributors.
 */

#include "main.h"

NxMode component;

void NxMode::handleSpectating(IPlayer &player)
{
	// NxPlayerData *data = queryExtension<NxPlayerData>(player);
}

void NxMode::onDialogResponse(IPlayer &player, int dialogId, DialogResponse response, int listItem, StringView inputText)
{
	dialogLogin(player, dialogId, response, listItem, inputText);
}

void NxMode::onPlayerConnect(IPlayer &player)
{
	player.addExtension(new NxPlayerData(), true);
	player.sendGameText("~r~open.mp~w~: NX CPP", Seconds(2), 1);
}

bool NxMode::onPlayerRequestClass(IPlayer &player, unsigned int classId)
{
	player.setPosition(Vector3(1984.4445f, 157.9501f, 55.9384f));
	player.setCameraLookAt(Vector3(1984.4445f, 157.9501f, 55.9384f), 2);
	player.setCameraPosition(Vector3(1984.4445f, 160.9501f, 55.9384f));
	player.setRotation(GTAQuat(0.0f, 0.0f, 0.0f));

	showDialogLogin(player);
	return true;
}

void NxMode::onVehicleStreamIn(IVehicle &vehicle, IPlayer &player)
{
	VehicleParams objective;
	objective.objective = 1;
}

void NxMode::onPlayerSpawn(IPlayer &player)
{
	NxPlayerData *data = queryExtension<NxPlayerData>(player);
	IPlayerDialogData *dialog = queryExtension<IPlayerDialogData>(player);

	if (data->loggedin == FALSE)
	{
		dialog->hide(player);
		player.kick();
	}
	else
	{
		std::string text = std::string("~b~Welcome ~r~") + std::string(player.getName());
		player.sendGameText(StringView(text), Seconds(6), 1);

		player.setPosition(Vector3(2200.8652f, 1392.9286f, 10.8203f));
		player.setRotation(GTAQuat(0.0f, 0.0f, 179.8621f));

		player.setCameraBehind();
	}	
}

void NxMode::onPlayerStateChange(IPlayer &player, PlayerState newState, PlayerState oldState)
{
	if (newState == PlayerState_Driver)
	{
		// IPlayerVehicleData *data = queryExtension<IPlayerVehicleData>(player);
	}
	else if (oldState == PlayerState_Driver)
	{
		queryExtension<IPlayerCheckpointData>(player)->getCheckpoint().disable();
	}
}

void NxMode::onPlayerEnterCheckpoint(IPlayer &player)
{
	if (player.getState() == PlayerState_Driver)
	{
		// IVehicle *vehicle = queryExtension<IPlayerVehicleData>(player)->getVehicle();
	}
}

void NxMode::onPlayerDeath(IPlayer &player, IPlayer *killer, int reason)
{
	// c->getPlayers().sendDeathMessageToAll(killer, player, reason);

	if (killer)
	{
		if (killer->getTeam() != player.getTeam())
		{
			killer->setScore(killer->getScore() + 1);
		}
	}

	NxPlayerData *data = queryExtension<NxPlayerData>(player);
	data->lastKiller = killer;
	data->lastDeath = Time::now();
}

void NxMode::onLoad(ICore *core)
{
	c = core;
	c->getPlayers().getPlayerConnectDispatcher().addEventHandler(this);
	c->getPlayers().getPlayerSpawnDispatcher().addEventHandler(this);
	c->getPlayers().getPlayerChangeDispatcher().addEventHandler(this);
	c->getPlayers().getPlayerDamageDispatcher().addEventHandler(this);
	c->getPlayers().getPlayerUpdateDispatcher().addEventHandler(this);
}

void NxMode::onInit(IComponentList *components)
{
	InitMySQL();

	std::cout << " " << std::endl;
	std::cout << "_______________________________________________" << std::endl;
	std::cout << "||-------------------------------------------||" << std::endl;
	std::cout << "|| The open.mp server is running. [C++ API]  ||" << std::endl;
	std::cout << "||                                           ||" << std::endl;
	std::cout << "|| [+] - MYSQL Connector/C++" << (conn ? " [connected]     ||" : " [error]     ||") << std::endl;
	std::cout << "|| [+] - Encryption [Alway running]          ||" << std::endl;
	std::cout << "||                                           ||" << std::endl;
	std::cout << "|| made by EasyCore.                         ||" << std::endl;
	std::cout << "||___________________________________________||" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << " " << std::endl;

	classes = components->queryComponent<IClassesComponent>();
	vehicles = components->queryComponent<IVehiclesComponent>();
	objects = components->queryComponent<IObjectsComponent>();
	checkpoints = components->queryComponent<ICheckpointsComponent>();
	timers = components->queryComponent<ITimersComponent>();
	dialogs = components->queryComponent<IDialogsComponent>();

	if (classes)
	{
		classes->getEventDispatcher().addEventHandler(this);
	}
	if (vehicles)
	{
		vehicles->getEventDispatcher().addEventHandler(this);
	}
	if (checkpoints)
	{
		checkpoints->getEventDispatcher().addEventHandler(this);
	}

	if (dialogs)
	{
		dialogs->getEventDispatcher().addEventHandler(this);
	}

	if (classes && vehicles && objects)
	{
		// vehicles->create(false, 400, Vector3(1948.7397f ,167.6365f ,37.2813f ), 73.2185f, 2, 2, Seconds(100));
	}
}

bool NxMode::onPlayerUpdate(IPlayer &player, TimePoint now)
{
	if (player.getState() == PlayerState_OnFoot)
	{
		const Vector3 pos = player.getPosition();
		Vector3 resupply1 = pos - Vector3(2140.83f, -235.13f, 7.13f);
		Vector3 resupply2 = pos - Vector3(2318.73f, 590.96, 6.75);
		
		if (glm::dot(resupply1, resupply1) < 2.5f || glm::dot(resupply2, resupply2) < 2.5f)
		{
			// NxPlayerData *data = queryExtension<NxPlayerData>(player);
		}
	}

	else if (player.getState() == PlayerState_Spectating)
	{
		// NxPlayerData *data = queryExtension<NxPlayerData>(player);
	}
	return true;
}

COMPONENT_ENTRY_POINT()
{
	return &component;
}