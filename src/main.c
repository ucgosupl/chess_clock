void main(void)
{
    while(1)
    {
        pin_state_t state = hw_heartbeat_pin_get();
        heartbeat_check(state);
    }
    
}