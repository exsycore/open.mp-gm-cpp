#pragma once

#include <Server/Components/Checkpoints/checkpoints.hpp>
#include <Server/Components/Classes/classes.hpp>
#include <Server/Components/Objects/objects.hpp>
#include <Server/Components/Timers/timers.hpp>
#include <Server/Components/Vehicles/vehicles.hpp>
#include <Server/Components/Dialogs/dialogs.hpp>

#include <sdk.hpp>

#include "dialogs/dialogs.h"
#include "mysql/mysql.h"
#include "utils/utils.h"

struct NxPlayerData final : public IExtension
{
	PROVIDE_EXT_UID(0x3544DA171ACA7553);

	void freeExtension() override
	{
		delete this;
	}

	void reset() override {}

	TimePoint lastResupplyTime;
	IPlayer *lastKiller = nullptr;
	TimePoint lastDeath;

	int id;
	std::string name;
	std::string password;
	std::string hash;
	bool loggedin = false;
};

struct NxMode : public IComponent,
						public PlayerConnectEventHandler,
						public PlayerSpawnEventHandler,
						public PlayerChangeEventHandler,
						public PlayerDamageEventHandler,
						public ClassEventHandler,
						public VehicleEventHandler,
						public PlayerCheckpointEventHandler,
						public PlayerUpdateEventHandler,
						public PlayerDialogEventHandler
{
	ICore *c = nullptr;
	IVehiclesComponent *vehicles = nullptr;
	IClassesComponent *classes = nullptr;
	IObjectsComponent *objects = nullptr;
	ICheckpointsComponent *checkpoints = nullptr;
	ITimersComponent *timers = nullptr;
	IDialogsComponent *dialogs = nullptr;

	UID getUID() override
	{
		return 0x3544DA171ACA7553;
	}

	SemanticVersion componentVersion() const override
	{
		return SemanticVersion(1, 0, 0, 0);
	}

	StringView componentName() const override
	{
		return "Nexura Cpp";
	}

	void resetGame() {}

	void handleSpectating(IPlayer &player);

	void onDialogResponse(IPlayer &player, int dialogId, DialogResponse response, int listItem, StringView inputText) override;

	void onPlayerConnect(IPlayer &player) override;

	bool onPlayerRequestClass(IPlayer &player, unsigned int classId) override;

	void onVehicleStreamIn(IVehicle &vehicle, IPlayer &player) override;

	void onPlayerSpawn(IPlayer &player) override;

	void onPlayerStateChange(IPlayer &player, PlayerState newState, PlayerState oldState) override;

	void onPlayerEnterCheckpoint(IPlayer &player) override;

	void onPlayerDeath(IPlayer &player, IPlayer *killer, int reason) override;

	void onLoad(ICore *core) override;

	void onInit(IComponentList *components) override;

	bool onPlayerUpdate(IPlayer &player, TimePoint now) override;

	NxMode()
	{
	}

	~NxMode()
	{
		if (c)
		{
			c->getPlayers().getPlayerConnectDispatcher().removeEventHandler(this);
			c->getPlayers().getPlayerSpawnDispatcher().removeEventHandler(this);
			c->getPlayers().getPlayerChangeDispatcher().removeEventHandler(this);
			c->getPlayers().getPlayerDamageDispatcher().removeEventHandler(this);
			c->getPlayers().getPlayerUpdateDispatcher().removeEventHandler(this);

			if (classes)
			{
				classes->getEventDispatcher().removeEventHandler(this);
			}
			if (vehicles)
			{
				vehicles->getEventDispatcher().removeEventHandler(this);
			}
			if (checkpoints)
			{
				checkpoints->getEventDispatcher().removeEventHandler(this);
			}
			if (dialogs)
			{
				dialogs->getEventDispatcher().removeEventHandler(this);
			}
		}
	}

	void free() override
	{
		delete this;
	}

	void reset() override
	{
	}

};

extern NxMode component;