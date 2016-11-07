# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=armv6
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.5 -DJUCE_APP_VERSION_HEX=0x10005 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I$(HOME)/Dev/JUCE/modules -I../../External/serial/include
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt 

  TARGET := LGML
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=armv6
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.5 -DJUCE_APP_VERSION_HEX=0x10005 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I$(HOME)/Dev/JUCE/modules
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt 

  TARGET := LGML
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Ubuntu)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Ubuntu
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.5 -DJUCE_APP_VERSION_HEX=0x10005 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I$(HOME)/Dev/JUCE/modules -I../../External/serial/include -I../../Source
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O2
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt 

  TARGET := LGML
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS := \
  $(JUCE_OBJDIR)/LooperTest_184b8746.o \
  $(JUCE_OBJDIR)/NodeChildProofer_7bc2479a.o \
  $(JUCE_OBJDIR)/Outliner_324682f9.o \
  $(JUCE_OBJDIR)/PresetManager_fc30642f.o \
  $(JUCE_OBJDIR)/PresetChooser_96a32abd.o \
  $(JUCE_OBJDIR)/ControllableUI_c544de6.o \
  $(JUCE_OBJDIR)/TriggerBlinkUI_693175a1.o \
  $(JUCE_OBJDIR)/TriggerButtonUI_d1a8d47f.o \
  $(JUCE_OBJDIR)/TriggerUI_2ce661ad.o \
  $(JUCE_OBJDIR)/ControllableHelpers_912c061d.o \
  $(JUCE_OBJDIR)/EnumParameterUI_e576769d.o \
  $(JUCE_OBJDIR)/ParameterProxyUI_7009c7a2.o \
  $(JUCE_OBJDIR)/FloatStepperUI_bb9cf532.o \
  $(JUCE_OBJDIR)/BoolToggleUI_bb303d7b.o \
  $(JUCE_OBJDIR)/FloatSliderUI_d00e5892.o \
  $(JUCE_OBJDIR)/IntSliderUI_a48244a5.o \
  $(JUCE_OBJDIR)/IntStepperUI_75a68b7f.o \
  $(JUCE_OBJDIR)/ParameterUI_37f5cbde.o \
  $(JUCE_OBJDIR)/StringParameterUI_aac76e8d.o \
  $(JUCE_OBJDIR)/BoolParameter_60c297c0.o \
  $(JUCE_OBJDIR)/EnumParameter_6e309b09.o \
  $(JUCE_OBJDIR)/FloatParameter_67c285b6.o \
  $(JUCE_OBJDIR)/IntParameter_21cc1c03.o \
  $(JUCE_OBJDIR)/Parameter_73564c8a.o \
  $(JUCE_OBJDIR)/ParameterProxy_74fe694e.o \
  $(JUCE_OBJDIR)/Point2DParameter_7a5034f0.o \
  $(JUCE_OBJDIR)/Point3DParameter_d42b9f31.o \
  $(JUCE_OBJDIR)/StringParameter_fe3ecef9.o \
  $(JUCE_OBJDIR)/Controllable_d3fdb092.o \
  $(JUCE_OBJDIR)/ControllableContainer_f3a7c159.o \
  $(JUCE_OBJDIR)/ControllableIndexedContainer_a91d6542.o \
  $(JUCE_OBJDIR)/Trigger_dbc00a19.o \
  $(JUCE_OBJDIR)/SerialManager_3fa6f9fa.o \
  $(JUCE_OBJDIR)/SerialPort_11c2339e.o \
  $(JUCE_OBJDIR)/SerialUIHelper_464101df.o \
  $(JUCE_OBJDIR)/ProgressNotifier_22cad7fc.o \
  $(JUCE_OBJDIR)/MIDIListener_ec38fcde.o \
  $(JUCE_OBJDIR)/MIDIManager_ba3551cd.o \
  $(JUCE_OBJDIR)/MIDIUIHelper_1d7da46c.o \
  $(JUCE_OBJDIR)/JsEnvironmentUI_eb77ea7f.o \
  $(JUCE_OBJDIR)/JsLiveEditor_98e58f2b.o \
  $(JUCE_OBJDIR)/JsContainerSync_318a26d4.o \
  $(JUCE_OBJDIR)/JsEnvironment_3c619f6b.o \
  $(JUCE_OBJDIR)/JsGlobalEnvironment_c35c5308.o \
  $(JUCE_OBJDIR)/SerialController_3b6eab39.o \
  $(JUCE_OBJDIR)/SerialControllerEditor_48dab746.o \
  $(JUCE_OBJDIR)/SerialControllerUI_5b713acd.o \
  $(JUCE_OBJDIR)/ControllerManagerUI_7e8032a6.o \
  $(JUCE_OBJDIR)/ControllerUIHelpers_d1429d16.o \
  $(JUCE_OBJDIR)/ControlVariable_ab8faeda.o \
  $(JUCE_OBJDIR)/ControlVariableUI_475fda2e.o \
  $(JUCE_OBJDIR)/Controller_2961cb25.o \
  $(JUCE_OBJDIR)/ControllerEditor_59f93632.o \
  $(JUCE_OBJDIR)/ControllerUI_991c0fb9.o \
  $(JUCE_OBJDIR)/DMXController_ce875bec.o \
  $(JUCE_OBJDIR)/MIDIController_360cde86.o \
  $(JUCE_OBJDIR)/MIDIControllerEditor_1c067cd3.o \
  $(JUCE_OBJDIR)/MIDIControllerUI_274fceda.o \
  $(JUCE_OBJDIR)/JavascriptController_d1e074f2.o \
  $(JUCE_OBJDIR)/JavascriptControllerEditor_8572923f.o \
  $(JUCE_OBJDIR)/JavascriptControllerUI_1c977a46.o \
  $(JUCE_OBJDIR)/OSCDirectController_935da9e5.o \
  $(JUCE_OBJDIR)/OSCCustomController_ec14cb6d.o \
  $(JUCE_OBJDIR)/OSCCustomControllerEditor_eb2ce07a.o \
  $(JUCE_OBJDIR)/OSCCustomControllerUI_7b101e01.o \
  $(JUCE_OBJDIR)/OSCControllerUI_5eeed4b0.o \
  $(JUCE_OBJDIR)/OSCControllerEditor_be62a7a9.o \
  $(JUCE_OBJDIR)/OSCController_303f61dc.o \
  $(JUCE_OBJDIR)/ControlManager_886b319.o \
  $(JUCE_OBJDIR)/FastMapperUI_9e2f9d1a.o \
  $(JUCE_OBJDIR)/FastMapUI_bf555555.o \
  $(JUCE_OBJDIR)/FastMap_2927c7c1.o \
  $(JUCE_OBJDIR)/FastMapper_f30b3cc6.o \
  $(JUCE_OBJDIR)/ScriptedCondition_af318e72.o \
  $(JUCE_OBJDIR)/ScriptedConditionUI_ea0833c6.o \
  $(JUCE_OBJDIR)/ScriptedConsequence_2ad9a41a.o \
  $(JUCE_OBJDIR)/ScriptedConsequenceUI_1c017f6e.o \
  $(JUCE_OBJDIR)/ControlVariableReferenceUI_e5de40ef.o \
  $(JUCE_OBJDIR)/RuleEditor_646e2092.o \
  $(JUCE_OBJDIR)/RuleConditionGroupUI_8bb2995d.o \
  $(JUCE_OBJDIR)/RuleConditionUI_d4953b34.o \
  $(JUCE_OBJDIR)/RuleConsequenceUI_976a635c.o \
  $(JUCE_OBJDIR)/RuleManagerUI_5a16ac46.o \
  $(JUCE_OBJDIR)/RuleUI_fc652a19.o \
  $(JUCE_OBJDIR)/ControlVariableReference_8c4ad1db.o \
  $(JUCE_OBJDIR)/Rule_beedfd85.o \
  $(JUCE_OBJDIR)/RuleCondition_e2959960.o \
  $(JUCE_OBJDIR)/RuleConditionGroup_96966dc9.o \
  $(JUCE_OBJDIR)/RuleConsequence_1566ab88.o \
  $(JUCE_OBJDIR)/RuleManager_698426f2.o \
  $(JUCE_OBJDIR)/NodeBase_eef060fc.o \
  $(JUCE_OBJDIR)/ConnectableNode_7ac32ac7.o \
  $(JUCE_OBJDIR)/NodeContainer_ab155880.o \
  $(JUCE_OBJDIR)/NodeContainerContentUI_a3d0d777.o \
  $(JUCE_OBJDIR)/NodeContainerViewer_323a4972.o \
  $(JUCE_OBJDIR)/NodeContainerUI_7c21b254.o \
  $(JUCE_OBJDIR)/NodeBaseAudioCtlUI_23419795.o \
  $(JUCE_OBJDIR)/NodeBaseContentUI_25719a7b.o \
  $(JUCE_OBJDIR)/NodeBaseHeaderUI_69c3237d.o \
  $(JUCE_OBJDIR)/NodeBaseUI_355c8bd0.o \
  $(JUCE_OBJDIR)/ConnectableNodeContentUI_25eda510.o \
  $(JUCE_OBJDIR)/ConnectableNodeAudioCtlUI_3246dfa0.o \
  $(JUCE_OBJDIR)/ConnectableNodeHeaderUI_5100f248.o \
  $(JUCE_OBJDIR)/ConnectableNodeUI_17a40edb.o \
  $(JUCE_OBJDIR)/Data_fc0f4f33.o \
  $(JUCE_OBJDIR)/DataProcessorGraph_756098af.o \
  $(JUCE_OBJDIR)/NodeConnectionEditor_faed6a16.o \
  $(JUCE_OBJDIR)/NodeConnectionEditorDataSlot_356891fe.o \
  $(JUCE_OBJDIR)/NodeConnectionEditorLink_f98d58b0.o \
  $(JUCE_OBJDIR)/ConnectorComponent_91ce38b9.o \
  $(JUCE_OBJDIR)/NodeConnection_d2f3ba09.o \
  $(JUCE_OBJDIR)/NodeConnectionUI_25edd59d.o \
  $(JUCE_OBJDIR)/JsNode_314f14f4.o \
  $(JUCE_OBJDIR)/JsNodeUI_5ade21c8.o \
  $(JUCE_OBJDIR)/ContainerOutNode_8c5d19b8.o \
  $(JUCE_OBJDIR)/ContainerInNode_4e507909.o \
  $(JUCE_OBJDIR)/AudioDeviceInNode_cb757d94.o \
  $(JUCE_OBJDIR)/AudioDeviceInNodeUI_50ce268.o \
  $(JUCE_OBJDIR)/AudioDeviceOutNode_b3d8a68d.o \
  $(JUCE_OBJDIR)/AudioDeviceOutNodeUI_6149b121.o \
  $(JUCE_OBJDIR)/DataInNode_dd21ae1a.o \
  $(JUCE_OBJDIR)/DataInNodeUI_5c6f096e.o \
  $(JUCE_OBJDIR)/DummyNode_fd1bb32b.o \
  $(JUCE_OBJDIR)/DummyNodeContentUI_375e4d2c.o \
  $(JUCE_OBJDIR)/AudioMixerNode_60d9bf7e.o \
  $(JUCE_OBJDIR)/AudioMixerNodeUI_d26851d2.o \
  $(JUCE_OBJDIR)/LooperNode_9342affc.o \
  $(JUCE_OBJDIR)/LooperNodeUI_e571ad0.o \
  $(JUCE_OBJDIR)/LooperTrack_e7785edb.o \
  $(JUCE_OBJDIR)/Spat3DViewer_17a1d23c.o \
  $(JUCE_OBJDIR)/Spat2DViewer_ff02cdd.o \
  $(JUCE_OBJDIR)/Spat2DNode_47c47e4d.o \
  $(JUCE_OBJDIR)/Spat2DNodeUI_a99e98e1.o \
  $(JUCE_OBJDIR)/PluginWindow_609dbc0c.o \
  $(JUCE_OBJDIR)/VSTManager_134ffcdf.o \
  $(JUCE_OBJDIR)/VSTNode_c6d00fda.o \
  $(JUCE_OBJDIR)/VSTNodeUI_940bfb2e.o \
  $(JUCE_OBJDIR)/NodeFactory_4513bb09.o \
  $(JUCE_OBJDIR)/NodeManagerUI_ff28af00.o \
  $(JUCE_OBJDIR)/NodeManager_42cb902c.o \
  $(JUCE_OBJDIR)/LGMLLogger_4146caf3.o \
  $(JUCE_OBJDIR)/LGMLLoggerUI_4bc05407.o \
  $(JUCE_OBJDIR)/ControllableEditor_dd5c95df.o \
  $(JUCE_OBJDIR)/CustomEditor_3b5461c7.o \
  $(JUCE_OBJDIR)/GenericControllableContainerEditor_117ebfb7.o \
  $(JUCE_OBJDIR)/InspectableComponent_72aa9838.o \
  $(JUCE_OBJDIR)/Inspector_d6237cd8.o \
  $(JUCE_OBJDIR)/InspectorEditor_dded95a5.o \
  $(JUCE_OBJDIR)/TimeManager_75588e81.o \
  $(JUCE_OBJDIR)/TimeManagerUI_c26f6c15.o \
  $(JUCE_OBJDIR)/TimeMasterCandidate_6cba9f15.o \
  $(JUCE_OBJDIR)/ProjectManager_ec121fdd.o \
  $(JUCE_OBJDIR)/SettingsManager_f0565d6b.o \
  $(JUCE_OBJDIR)/ShapeShifterFactory_6c2ae3bd.o \
  $(JUCE_OBJDIR)/ShapeShifterPanelHeader_8b4f9de4.o \
  $(JUCE_OBJDIR)/ShapeShifterPanelTab_a4791be8.o \
  $(JUCE_OBJDIR)/ShapeShifterContent_505997ac.o \
  $(JUCE_OBJDIR)/ShapeShifter_bf2b9b57.o \
  $(JUCE_OBJDIR)/GapGrabber_4d565216.o \
  $(JUCE_OBJDIR)/ShapeShifterWindow_e5e759e7.o \
  $(JUCE_OBJDIR)/ShapeShifterPanel_821dd737.o \
  $(JUCE_OBJDIR)/ShapeShifterContainer_69052434.o \
  $(JUCE_OBJDIR)/ShapeShifterManager_69e2b8e0.o \
  $(JUCE_OBJDIR)/BetterStepper_8f8519a2.o \
  $(JUCE_OBJDIR)/LookAndFeelOO_b6d1a33f.o \
  $(JUCE_OBJDIR)/ProgressWindow_8ee8f986.o \
  $(JUCE_OBJDIR)/Engine_7f3228cb.o \
  $(JUCE_OBJDIR)/EngineFileDocument_abfb722.o \
  $(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o \
  $(JUCE_OBJDIR)/MainComponentCommands_b1179f4d.o \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/list_ports_linux_698dd22b.o \
  $(JUCE_OBJDIR)/list_ports_osx_96f9b04b.o \
  $(JUCE_OBJDIR)/list_ports_win_a40efe83.o \
  $(JUCE_OBJDIR)/unix_2bfcd685.o \
  $(JUCE_OBJDIR)/win_d3034fd3.o \
  $(JUCE_OBJDIR)/serial_ecdf186c.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o \
  $(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o \
  $(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o \
  $(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o \
  $(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o \
  $(JUCE_OBJDIR)/juce_core_75b14332.o \
  $(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o \
  $(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o \
  $(JUCE_OBJDIR)/juce_events_d2be882c.o \
  $(JUCE_OBJDIR)/juce_graphics_9c18891e.o \
  $(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o \
  $(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o \
  $(JUCE_OBJDIR)/juce_osc_be4cee16.o \

.PHONY: clean

$(JUCE_OUTDIR)/$(TARGET): check-pkg-config $(OBJECTS) $(RESOURCES)
	@echo Linking LGML
	-@mkdir -p $(JUCE_BINDIR)
	-@mkdir -p $(JUCE_LIBDIR)
	-@mkdir -p $(JUCE_OUTDIR)
	@$(BLDCMD)

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl x11 xext xinerama

clean:
	@echo Cleaning LGML
	@$(CLEANCMD)

strip:
	@echo Stripping LGML
	-@$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

$(JUCE_OBJDIR)/LooperTest_184b8746.o: ../../Tests/LooperTest.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LooperTest.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeChildProofer_7bc2479a.o: ../../Tests/NodeChildProofer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeChildProofer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Outliner_324682f9.o: ../../Source/Outliner.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Outliner.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PresetManager_fc30642f.o: ../../Source/PresetManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PresetManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PresetChooser_96a32abd.o: ../../Source/PresetChooser.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PresetChooser.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableUI_c544de6.o: ../../Source/ControllableUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerBlinkUI_693175a1.o: ../../Source/TriggerBlinkUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerBlinkUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerButtonUI_d1a8d47f.o: ../../Source/TriggerButtonUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerButtonUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerUI_2ce661ad.o: ../../Source/TriggerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableHelpers_912c061d.o: ../../Source/ControllableHelpers.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableHelpers.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EnumParameterUI_e576769d.o: ../../Source/EnumParameterUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EnumParameterUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ParameterProxyUI_7009c7a2.o: ../../Source/ParameterProxyUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ParameterProxyUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FloatStepperUI_bb9cf532.o: ../../Source/FloatStepperUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FloatStepperUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BoolToggleUI_bb303d7b.o: ../../Source/BoolToggleUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BoolToggleUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FloatSliderUI_d00e5892.o: ../../Source/FloatSliderUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FloatSliderUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IntSliderUI_a48244a5.o: ../../Source/IntSliderUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IntSliderUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IntStepperUI_75a68b7f.o: ../../Source/IntStepperUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IntStepperUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ParameterUI_37f5cbde.o: ../../Source/ParameterUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ParameterUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StringParameterUI_aac76e8d.o: ../../Source/StringParameterUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StringParameterUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BoolParameter_60c297c0.o: ../../Source/BoolParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BoolParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EnumParameter_6e309b09.o: ../../Source/EnumParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EnumParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FloatParameter_67c285b6.o: ../../Source/FloatParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FloatParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IntParameter_21cc1c03.o: ../../Source/IntParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IntParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Parameter_73564c8a.o: ../../Source/Parameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Parameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ParameterProxy_74fe694e.o: ../../Source/ParameterProxy.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ParameterProxy.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Point2DParameter_7a5034f0.o: ../../Source/Point2DParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Point2DParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Point3DParameter_d42b9f31.o: ../../Source/Point3DParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Point3DParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StringParameter_fe3ecef9.o: ../../Source/StringParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StringParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Controllable_d3fdb092.o: ../../Source/Controllable.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Controllable.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableContainer_f3a7c159.o: ../../Source/ControllableContainer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableContainer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableIndexedContainer_a91d6542.o: ../../Source/ControllableIndexedContainer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableIndexedContainer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Trigger_dbc00a19.o: ../../Source/Trigger.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Trigger.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialManager_3fa6f9fa.o: ../../Source/SerialManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialPort_11c2339e.o: ../../Source/SerialPort.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialPort.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialUIHelper_464101df.o: ../../Source/SerialUIHelper.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialUIHelper.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProgressNotifier_22cad7fc.o: ../../Source/ProgressNotifier.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProgressNotifier.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIListener_ec38fcde.o: ../../Source/MIDIListener.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIListener.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIManager_ba3551cd.o: ../../Source/MIDIManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIUIHelper_1d7da46c.o: ../../Source/MIDIUIHelper.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIUIHelper.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JsEnvironmentUI_eb77ea7f.o: ../../Source/JsEnvironmentUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JsEnvironmentUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JsLiveEditor_98e58f2b.o: ../../Source/JsLiveEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JsLiveEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JsContainerSync_318a26d4.o: ../../Source/JsContainerSync.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JsContainerSync.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JsEnvironment_3c619f6b.o: ../../Source/JsEnvironment.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JsEnvironment.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JsGlobalEnvironment_c35c5308.o: ../../Source/JsGlobalEnvironment.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JsGlobalEnvironment.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialController_3b6eab39.o: ../../Source/SerialController.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialController.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialControllerEditor_48dab746.o: ../../Source/SerialControllerEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialControllerEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialControllerUI_5b713acd.o: ../../Source/SerialControllerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialControllerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllerManagerUI_7e8032a6.o: ../../Source/ControllerManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllerManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllerUIHelpers_d1429d16.o: ../../Source/ControllerUIHelpers.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllerUIHelpers.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControlVariable_ab8faeda.o: ../../Source/ControlVariable.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControlVariable.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControlVariableUI_475fda2e.o: ../../Source/ControlVariableUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControlVariableUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Controller_2961cb25.o: ../../Source/Controller.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Controller.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllerEditor_59f93632.o: ../../Source/ControllerEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllerEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllerUI_991c0fb9.o: ../../Source/ControllerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXController_ce875bec.o: ../../Source/DMXController.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXController.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIController_360cde86.o: ../../Source/MIDIController.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIController.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIControllerEditor_1c067cd3.o: ../../Source/MIDIControllerEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIControllerEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIControllerUI_274fceda.o: ../../Source/MIDIControllerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIControllerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JavascriptController_d1e074f2.o: ../../Source/JavascriptController.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JavascriptController.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JavascriptControllerEditor_8572923f.o: ../../Source/JavascriptControllerEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JavascriptControllerEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JavascriptControllerUI_1c977a46.o: ../../Source/JavascriptControllerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JavascriptControllerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCDirectController_935da9e5.o: ../../Source/OSCDirectController.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCDirectController.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCCustomController_ec14cb6d.o: ../../Source/OSCCustomController.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCCustomController.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCCustomControllerEditor_eb2ce07a.o: ../../Source/OSCCustomControllerEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCCustomControllerEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCCustomControllerUI_7b101e01.o: ../../Source/OSCCustomControllerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCCustomControllerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCControllerUI_5eeed4b0.o: ../../Source/OSCControllerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCControllerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCControllerEditor_be62a7a9.o: ../../Source/OSCControllerEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCControllerEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCController_303f61dc.o: ../../Source/OSCController.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCController.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControlManager_886b319.o: ../../Source/ControlManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControlManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FastMapperUI_9e2f9d1a.o: ../../Source/FastMapperUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FastMapperUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FastMapUI_bf555555.o: ../../Source/FastMapUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FastMapUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FastMap_2927c7c1.o: ../../Source/FastMap.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FastMap.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FastMapper_f30b3cc6.o: ../../Source/FastMapper.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FastMapper.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScriptedCondition_af318e72.o: ../../Source/ScriptedCondition.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScriptedCondition.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScriptedConditionUI_ea0833c6.o: ../../Source/ScriptedConditionUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScriptedConditionUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScriptedConsequence_2ad9a41a.o: ../../Source/ScriptedConsequence.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScriptedConsequence.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScriptedConsequenceUI_1c017f6e.o: ../../Source/ScriptedConsequenceUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScriptedConsequenceUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControlVariableReferenceUI_e5de40ef.o: ../../Source/ControlVariableReferenceUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControlVariableReferenceUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RuleEditor_646e2092.o: ../../Source/RuleEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RuleEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RuleConditionGroupUI_8bb2995d.o: ../../Source/RuleConditionGroupUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RuleConditionGroupUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RuleConditionUI_d4953b34.o: ../../Source/RuleConditionUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RuleConditionUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RuleConsequenceUI_976a635c.o: ../../Source/RuleConsequenceUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RuleConsequenceUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RuleManagerUI_5a16ac46.o: ../../Source/RuleManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RuleManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RuleUI_fc652a19.o: ../../Source/RuleUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RuleUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControlVariableReference_8c4ad1db.o: ../../Source/ControlVariableReference.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControlVariableReference.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Rule_beedfd85.o: ../../Source/Rule.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Rule.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RuleCondition_e2959960.o: ../../Source/RuleCondition.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RuleCondition.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RuleConditionGroup_96966dc9.o: ../../Source/RuleConditionGroup.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RuleConditionGroup.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RuleConsequence_1566ab88.o: ../../Source/RuleConsequence.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RuleConsequence.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RuleManager_698426f2.o: ../../Source/RuleManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RuleManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeBase_eef060fc.o: ../../Source/NodeBase.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeBase.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConnectableNode_7ac32ac7.o: ../../Source/ConnectableNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConnectableNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeContainer_ab155880.o: ../../Source/NodeContainer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeContainer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeContainerContentUI_a3d0d777.o: ../../Source/NodeContainerContentUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeContainerContentUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeContainerViewer_323a4972.o: ../../Source/NodeContainerViewer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeContainerViewer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeContainerUI_7c21b254.o: ../../Source/NodeContainerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeContainerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeBaseAudioCtlUI_23419795.o: ../../Source/NodeBaseAudioCtlUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeBaseAudioCtlUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeBaseContentUI_25719a7b.o: ../../Source/NodeBaseContentUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeBaseContentUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeBaseHeaderUI_69c3237d.o: ../../Source/NodeBaseHeaderUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeBaseHeaderUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeBaseUI_355c8bd0.o: ../../Source/NodeBaseUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeBaseUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConnectableNodeContentUI_25eda510.o: ../../Source/ConnectableNodeContentUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConnectableNodeContentUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConnectableNodeAudioCtlUI_3246dfa0.o: ../../Source/ConnectableNodeAudioCtlUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConnectableNodeAudioCtlUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConnectableNodeHeaderUI_5100f248.o: ../../Source/ConnectableNodeHeaderUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConnectableNodeHeaderUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConnectableNodeUI_17a40edb.o: ../../Source/ConnectableNodeUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConnectableNodeUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Data_fc0f4f33.o: ../../Source/Data.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Data.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DataProcessorGraph_756098af.o: ../../Source/DataProcessorGraph.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DataProcessorGraph.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeConnectionEditor_faed6a16.o: ../../Source/NodeConnectionEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeConnectionEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeConnectionEditorDataSlot_356891fe.o: ../../Source/NodeConnectionEditorDataSlot.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeConnectionEditorDataSlot.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeConnectionEditorLink_f98d58b0.o: ../../Source/NodeConnectionEditorLink.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeConnectionEditorLink.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConnectorComponent_91ce38b9.o: ../../Source/ConnectorComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConnectorComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeConnection_d2f3ba09.o: ../../Source/NodeConnection.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeConnection.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeConnectionUI_25edd59d.o: ../../Source/NodeConnectionUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeConnectionUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JsNode_314f14f4.o: ../../Source/JsNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JsNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JsNodeUI_5ade21c8.o: ../../Source/JsNodeUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JsNodeUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ContainerOutNode_8c5d19b8.o: ../../Source/ContainerOutNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ContainerOutNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ContainerInNode_4e507909.o: ../../Source/ContainerInNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ContainerInNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioDeviceInNode_cb757d94.o: ../../Source/AudioDeviceInNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioDeviceInNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioDeviceInNodeUI_50ce268.o: ../../Source/AudioDeviceInNodeUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioDeviceInNodeUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioDeviceOutNode_b3d8a68d.o: ../../Source/AudioDeviceOutNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioDeviceOutNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioDeviceOutNodeUI_6149b121.o: ../../Source/AudioDeviceOutNodeUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioDeviceOutNodeUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DataInNode_dd21ae1a.o: ../../Source/DataInNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DataInNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DataInNodeUI_5c6f096e.o: ../../Source/DataInNodeUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DataInNodeUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DummyNode_fd1bb32b.o: ../../Source/DummyNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DummyNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DummyNodeContentUI_375e4d2c.o: ../../Source/DummyNodeContentUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DummyNodeContentUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioMixerNode_60d9bf7e.o: ../../Source/AudioMixerNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioMixerNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioMixerNodeUI_d26851d2.o: ../../Source/AudioMixerNodeUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioMixerNodeUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LooperNode_9342affc.o: ../../Source/LooperNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LooperNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LooperNodeUI_e571ad0.o: ../../Source/LooperNodeUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LooperNodeUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LooperTrack_e7785edb.o: ../../Source/LooperTrack.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LooperTrack.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Spat3DViewer_17a1d23c.o: ../../Source/Spat3DViewer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Spat3DViewer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Spat2DViewer_ff02cdd.o: ../../Source/Spat2DViewer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Spat2DViewer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Spat2DNode_47c47e4d.o: ../../Source/Spat2DNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Spat2DNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Spat2DNodeUI_a99e98e1.o: ../../Source/Spat2DNodeUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Spat2DNodeUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PluginWindow_609dbc0c.o: ../../Source/PluginWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PluginWindow.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/VSTManager_134ffcdf.o: ../../Source/VSTManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling VSTManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/VSTNode_c6d00fda.o: ../../Source/VSTNode.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling VSTNode.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/VSTNodeUI_940bfb2e.o: ../../Source/VSTNodeUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling VSTNodeUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeFactory_4513bb09.o: ../../Source/NodeFactory.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeFactory.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeManagerUI_ff28af00.o: ../../Source/NodeManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NodeManager_42cb902c.o: ../../Source/NodeManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NodeManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LGMLLogger_4146caf3.o: ../../Source/LGMLLogger.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LGMLLogger.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LGMLLoggerUI_4bc05407.o: ../../Source/LGMLLoggerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LGMLLoggerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableEditor_dd5c95df.o: ../../Source/ControllableEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomEditor_3b5461c7.o: ../../Source/CustomEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GenericControllableContainerEditor_117ebfb7.o: ../../Source/GenericControllableContainerEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GenericControllableContainerEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InspectableComponent_72aa9838.o: ../../Source/InspectableComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InspectableComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Inspector_d6237cd8.o: ../../Source/Inspector.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Inspector.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InspectorEditor_dded95a5.o: ../../Source/InspectorEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InspectorEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeManager_75588e81.o: ../../Source/TimeManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeManagerUI_c26f6c15.o: ../../Source/TimeManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeMasterCandidate_6cba9f15.o: ../../Source/TimeMasterCandidate.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeMasterCandidate.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProjectManager_ec121fdd.o: ../../Source/ProjectManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProjectManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SettingsManager_f0565d6b.o: ../../Source/SettingsManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SettingsManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterFactory_6c2ae3bd.o: ../../Source/ShapeShifterFactory.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterFactory.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterPanelHeader_8b4f9de4.o: ../../Source/ShapeShifterPanelHeader.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterPanelHeader.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterPanelTab_a4791be8.o: ../../Source/ShapeShifterPanelTab.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterPanelTab.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterContent_505997ac.o: ../../Source/ShapeShifterContent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterContent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifter_bf2b9b57.o: ../../Source/ShapeShifter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GapGrabber_4d565216.o: ../../Source/GapGrabber.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GapGrabber.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterWindow_e5e759e7.o: ../../Source/ShapeShifterWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterWindow.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterPanel_821dd737.o: ../../Source/ShapeShifterPanel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterPanel.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterContainer_69052434.o: ../../Source/ShapeShifterContainer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterContainer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterManager_69e2b8e0.o: ../../Source/ShapeShifterManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BetterStepper_8f8519a2.o: ../../Source/BetterStepper.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BetterStepper.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LookAndFeelOO_b6d1a33f.o: ../../Source/LookAndFeelOO.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LookAndFeelOO.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProgressWindow_8ee8f986.o: ../../Source/ProgressWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProgressWindow.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Engine_7f3228cb.o: ../../Source/Engine.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Engine.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EngineFileDocument_abfb722.o: ../../Source/EngineFileDocument.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EngineFileDocument.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponentCommands_b1179f4d.o: ../../Source/MainComponentCommands.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainComponentCommands.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/list_ports_linux_698dd22b.o: ../../Source/libs/serial/impl/list_ports/list_ports_linux.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling list_ports_linux.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/list_ports_osx_96f9b04b.o: ../../Source/libs/serial/impl/list_ports/list_ports_osx.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling list_ports_osx.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/list_ports_win_a40efe83.o: ../../Source/libs/serial/impl/list_ports/list_ports_win.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling list_ports_win.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/unix_2bfcd685.o: ../../Source/libs/serial/impl/unix.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling unix.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/win_d3034fd3.o: ../../Source/libs/serial/impl/win.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling win.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/serial_ecdf186c.o: ../../Source/libs/serial/serial.cc
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling serial.cc"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o: ../../JuceLibraryCode/juce_audio_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o: ../../JuceLibraryCode/juce_audio_devices.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o: ../../JuceLibraryCode/juce_audio_formats.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o: ../../JuceLibraryCode/juce_audio_processors.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o: ../../JuceLibraryCode/juce_audio_utils.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o: ../../JuceLibraryCode/juce_cryptography.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o: ../../JuceLibraryCode/juce_opengl.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_osc_be4cee16.o: ../../JuceLibraryCode/juce_osc.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_osc.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
