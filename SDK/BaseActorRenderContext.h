// SDK/BaseActorRenderContext.h
#pragma once

class ScreenContext;
struct ClientInstance;
struct MinecraftGame;

class BaseActorRenderContext {
public:
    BaseActorRenderContext(ScreenContext* screenCtx, ClientInstance* client, MinecraftGame* game);
};
