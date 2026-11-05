# FreeRTOS task flowcharts

- InputTask: read GPIO -> debounce -> make InputEvent -> send queue -> delay until 10ms.
- GameTask: receive events -> validate moves -> gravity/lock/clear -> publish GameFrame -> delay until 20ms.
- DisplayTask: receive latest frame -> compare dirty cells -> SPI repaint -> delay until ~33ms.

Finalize Draw.io/Mermaid diagrams after drivers and engine are implemented.
