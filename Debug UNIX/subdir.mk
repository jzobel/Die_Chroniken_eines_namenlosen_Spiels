################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Basic3DObject.cpp \
../BasicHero.cpp \
../BasicLifeform.cpp \
../BasicStaticObject.cpp \
../BufferCullMeshSceneNode.cpp \
../Camera.cpp \
../Collision.cpp \
../Configuration.cpp \
../DebugShape.cpp \
../DebugShapesManager.cpp \
../DebugStatistics.cpp \
../Debugwindow.cpp \
../Eventreceiver.cpp \
../GameFloatControl.cpp \
../GameStateManager.cpp \
../GenericHelperMethods.cpp \
../Ground.cpp \
../Hero.cpp \
../HeroPunk.cpp \
../Logfile.cpp \
../MapTile.cpp \
../Mauspfeil.cpp \
../ObjectManager.cpp \
../ObjectParamsExtractor.cpp \
../SaveGames.cpp \
../Scripting.cpp \
../StateLoadGameContent.cpp \
../StateMainMenu.cpp \
../StatePlayTheGame.cpp \
../StateStartup.cpp \
../StateUnloadGameContent.cpp \
../Timer.cpp \
../TimerManager.cpp \
../Ton.cpp \
../Vegetation.cpp \
../Weather.cpp \
../Zufall.cpp \
../main.cpp 

OBJS += \
./Basic3DObject.o \
./BasicHero.o \
./BasicLifeform.o \
./BasicStaticObject.o \
./BufferCullMeshSceneNode.o \
./Camera.o \
./Collision.o \
./Configuration.o \
./DebugShape.o \
./DebugShapesManager.o \
./DebugStatistics.o \
./Debugwindow.o \
./Eventreceiver.o \
./GameFloatControl.o \
./GameStateManager.o \
./GenericHelperMethods.o \
./Ground.o \
./Hero.o \
./HeroPunk.o \
./Logfile.o \
./MapTile.o \
./Mauspfeil.o \
./ObjectManager.o \
./ObjectParamsExtractor.o \
./SaveGames.o \
./Scripting.o \
./StateLoadGameContent.o \
./StateMainMenu.o \
./StatePlayTheGame.o \
./StateStartup.o \
./StateUnloadGameContent.o \
./Timer.o \
./TimerManager.o \
./Ton.o \
./Vegetation.o \
./Weather.o \
./Zufall.o \
./main.o 

CPP_DEPS += \
./Basic3DObject.d \
./BasicHero.d \
./BasicLifeform.d \
./BasicStaticObject.d \
./BufferCullMeshSceneNode.d \
./Camera.d \
./Collision.d \
./Configuration.d \
./DebugShape.d \
./DebugShapesManager.d \
./DebugStatistics.d \
./Debugwindow.d \
./Eventreceiver.d \
./GameFloatControl.d \
./GameStateManager.d \
./GenericHelperMethods.d \
./Ground.d \
./Hero.d \
./HeroPunk.d \
./Logfile.d \
./MapTile.d \
./Mauspfeil.d \
./ObjectManager.d \
./ObjectParamsExtractor.d \
./SaveGames.d \
./Scripting.d \
./StateLoadGameContent.d \
./StateMainMenu.d \
./StatePlayTheGame.d \
./StateStartup.d \
./StateUnloadGameContent.d \
./Timer.d \
./TimerManager.d \
./Ton.d \
./Vegetation.d \
./Weather.d \
./Zufall.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_DEBUG_MODE=1 -DNULL=0 -Ihome/odonata/Projects/.workspace/Spiel_lib/irrlicht-1.8.1/include -Ihome/odonata/Projects/.workspace/Spiel_lib/irrKlang-1.4.0/include -Ihome/odonata/Projects/.workspace/Spiel_lib/irrlicht-1.8.1/source/Irrlicht -Iusr/include/lua5.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


