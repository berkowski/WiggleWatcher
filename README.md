# WiggleWatcher: APS1540 Magnetometer Logger

A simple data logger for the APS1540 magnetometer.

# Setup
## APS1540
The APS1540 must be configured to send data using the "ASCII Data Only" format:
```
  0l
  0wv1
```
It is also recommended to enable auto send mode so that the APS1540 starts
sending data immediately after powering up:

```
  0l
  0wc01b5a

  0l
  0wc08b10
```

## Logger
The configuration file for WiggleWatcher can be stored in a variety of locations.

- OSX:
  1. `config.toml` in the directory containing the `wigglewatcher` executable
  2. `~/Library/Preferences/wigglewatcher/config.toml`
  3. `~/Library/Preferences/wigglewatcher.toml`
  4. `~/wigglewatcher.toml`
- Linux:
  1. `config.toml` in the directory containing the `wigglewatcher` executable
  2. `~/.config/wigglewatcher/config.toml`
  3. `~/.config/wigglewatcher.toml`
  4. `~/wigglewatcher.toml`
- Windows:
  1. `config.toml` in the directory containing the `wigglewatcher` executable
  2. `C:/Users/$USER/AppData/Local/wigglewatcher/config.toml`
  3. `C:/ProgramData/wigglewatcher/config.toml`
  4. `C:/Users/$USER/AppData/Local/wigglewatcher.toml`
  5. `C:/Users/ProgramData/wigglewatcher.toml`
  6. `C:/Users/$USER/wigglewatcher.toml`

An error message will be showing the exact paths searched will be shown if no configuration
is found.  Look at `examples/config.toml` for a configuration example that can be used as
a template.
