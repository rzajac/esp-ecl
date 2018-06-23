# esp_eb

In event driven environment like ESP8266 event bus helps make your code less
"linear".

![EventBus](../../doc/EventBus.png)

With this library you will be able to define custom events and react to them. 
You will be able to attach / detach functions to handle events.
The additional feature is that all event callbacks are scheduled with timers 
not to block the CPU when there are many event listeners.
 
This library will allow you to:
 - create custom events
 - trigger custom events
 - attach / detach event listeners (callbacks)
 - pass arguments during event trigger
 
See [example program](../../examples/events) and library documentation in 
[esp_eb.h](esp_eb.h) header file for more details.
