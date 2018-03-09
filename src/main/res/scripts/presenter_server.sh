#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Start daemon in background
echo "Starting key sender daemon."
# Wait until the user has entered his password, but execute the daemon in background afterwards
pkexec /bin/bash -c "\"${SCRIPT_DIR}/keysenderDaemon/Presenter_Server_Keysender_Daemon\" &"

# Give the key sender some time to fire up
sleep 0.5

# Run the presenter server
"${SCRIPT_DIR}/main/Presenter_Server"
