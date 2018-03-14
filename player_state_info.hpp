#ifndef PLAYER_STATE_INFO
#define PLAYER_STATE_INFO

enum PlayerState{
	PLAYER_STATE_STANDING, PLAYER_STATE_MOVING, PLAYER_STATE_ATTACKING, PLAYER_STATE_DEAD
};

typedef struct PlayerStateInfo{
	PlayerState player_state_;
	
	int time_delayed_until_;
	int time_of_start_movement_;
	int time_of_death_;
} PlayerStateInfo;

#endif