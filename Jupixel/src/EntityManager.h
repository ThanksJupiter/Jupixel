#pragma once

// want shit like this
// bool for success?
// parameters for which components to add?
void add_entity();

// remove components associated with entity ID
void remove_entity(unsigned int ID);

// parameter for which component to add
// hardcode functions to things I know I want add/remove? yes probably veri good boi
void add_component_to_entity(unsigned int ID);

// delta time?
void update_entities();
