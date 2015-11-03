Access System Design
---

Use cases
===

The Access System is designed to enable the following use cases:
  * When an authorized card is presented to the card reader, the door latch is momentarily released, allowing entry
    to the building.
  * In response to the door latch being opened, a notification is sent to IRC announcing the event.
  * Authorized users may set the door latch to stay open for a fixed amount of time (e.g. enable public access
    during public demonstrations).
  * Authorized users may add/remove/modify users within the authorization system.
  * Authorized users may add/remove/modify authorization for cards to open the door latch.
  * Authorized users may manage the system through a web site.
  * Authorized users may manage the system through a local control panel within close proximity to the access system.
  * Authorized users may manage the system through a bot in an IRC channel.
  * Authorized users may check the history of events that lead to the door latch being opened, including which
    user requested it, when, and through which access method.
  * Authorized users may check the history of actions involving the authorization system, including requests to open
    the door, queries of user authorization, and modifications to the user authorization.

High-level
===

The system consists of eight major components:
  1. System Central Controller
  2. Authorization Service
  3. Logging Service
  4. Door Controller
  5. Card Reader Controller
  6. Front Panel UI
  7. Web UI
  8. IRC UI

### System Central Controller

The System Central Controller acts as the central control of the entire system. It takes events from the other
subsystems and requests actions from the other systems in response. It is effectively the business logic layer of
the system. For example, if the Card Reader Controller sends a "card read" event to the System Central Contoller,
the System Central Controller will send the event to the Logging Service to be logged, request door opening
authorization from the Authorization Service, and if access is granted, it will send a "momentarily open door"
command to the Door Controller.

The System Central Controller presents itself as a RESTful web API. Other subsystems register themselves with the
System Central Controller with their URI and the list of events that they want to be notified of. They can send
requests to the System Central Controller, either notifying it of an event or requesting an action to be taken,
and the System Central Controller will inform the other listening subsystems as appropriate in order for the event
to be handled or the action to be undertaken.

** TODO: Fill in nouns and verbs for API. **

### Authorization

The Authorization Service is responsible for authorizing actions in the system, such as opening the door and changing
user authorization. It is also responsible for logging such events and their outcomes, such as who requested the door
to open, when, by what access method, and whether or not they were authorized to do so.

The Authorization Service presents itself as a RESTful web API.

** TODO: Fill in nouns and verbs for API. **

### Logging Service

The Logging Service is responsible for maintaining the history of events within the system for the purpose of audit
and diagnosis. Events that are logged can include things like requests to open the door or changes to user
authorization. The Logging Service stores a record for every one of these events, including the event type, the time
and date at which it occurred, what user requested or generated it, and what subsystem originated the event. The
contents of this record can than be requested at a later time by other subsystems. For example, the web UI could
make a request for the last 10 door open events.

### Door Controller

The Door Controller provides a simple API for manipulating the latch on the door. It accepts commands to momentarily
open the door latch, to hold the door latch open for a set amount of time, or to close the door latch, overriding any
previous requests to open the door latch. It can also be polled for the current state of the door latch. It will emit
events specifying any requests made of it (for logging) as well as every time the latch changes state.

### Card Reader Controller

The Card Reader Controller listens for incoming signals from the card reader. When a card number is successfully
received, it informs the System Central Controller, which can then take any desired action (such as requesting the
door latch to be opened momentarily).

### Front Panel UI

The Front Panel UI is a user interface which is mounted within the building and is meant for immediate control fo the
system by the occupants of the building. It can be used to check the current state of the system, to review the history
of accesses, to request or cancel holding the door latch open for a fixed length of time, and to add/modify/remove
cards from the authorization database. It can also be used to reset the system in case of malfunction and to modify the
system software for repair or upgrades.

### Web UI

The Web UI is a website that can be accessed from any computer over the Internet. It requires authorization in order
to access it. It provides the same features as the Front Panel UI, including checking the current state of the system,
reviewing the history of accesses, requesting or canceling holding the door latch open, and adding, removing, or
modifying the authorization database. It is designed to allow remote management of the system in cases where the user
is not physically located within the building, but is needed to perform management tasks on the system.

### IRC UI

The IRC UI is an IRC bot that can be used to interact with the system through communication over IRC. It provides
notifications of certain events within the system (such as successful requests to open the door). It also allows
querying the history of events, such as displaying the last time the door was opened and for whom it was opened.
It is stateless and does not provide any authorization mechanism. For this reason, it is limited to only a subset
of functionality that does not require authorization. It is meant to be used primarily as a public notification system,
not as a remote management facility. Remote management should be performed through the Web UI.

