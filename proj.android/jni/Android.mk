LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

# ARMv7-A向けにもビルドする
APP_ABI := armeabi armeabi-v7a x86 arm64-v8a

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*/*/*.cpp)
#FILE_LIST := $(shell find $(LOCAL_PATH)/../../Classes -name *.cpp)

LOCAL_SRC_FILES := hellocpp/main.cpp
LOCAL_SRC_FILES += ../../Classes/External/Sqlite/sqlite3.c
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)


#CPP_FILES := $(shell find $(LOCAL_PATH)/../../Classes -name *.cpp)
#LOCAL_SRC_FILES := hellocpp/main.cpp
#LOCAL_SRC_FILES += $(CPP_FILES:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Definitions
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/External/Sqlite
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Manager
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Model
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Scene
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Scene/Court
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Scene/Member
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Scene/Overlay
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Scene/Participant
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Util
LOCAL_C_INCLUDES += $(shell find $(LOCAL_PATH)/../../Classes -type d)


# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

