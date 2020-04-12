#pragma once

#include "Components/NetworkComponent.h"

bool net_init();
void net_quit();

bool net_init_server_component(NetworkComponent& component);
bool net_init_client_component(NetworkComponent& component);
bool net_init_component(NetworkComponent& component);


