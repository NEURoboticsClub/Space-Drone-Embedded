/// @brief The states of the car
typedef enum
{
    /// @brief Unattached,  can attach
    READY,
    /// @brief Attaching to the MT
    ATTACHING,
    /// @brief Attached to the MT
    ATTACHED,
    /// @brief Detached from the MT
    DETACHING,
    /// @brief Faulted, see fault.h for fault codes
    FAULTED
} state_types_t;

/// @brief Initialize the state machine
esp_err_t state_init();

/// Queue a state transition to new a new state
esp_err_t queue_state_transition(state_types_t state);

/// Get the current state
state_types_t get_current_state();