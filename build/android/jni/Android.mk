#### Shared library for libmusicxml ####

LOCAL_PATH := $(call my-dir)
SRC := $(LOCAL_PATH)/../../../src

include $(CLEAR_VARS)

LOCAL_MODULE := musicxml2

# SRC var is a trick to use wildcard in android.mk
# if no wildcard, you have better to use relative path, conforming to android doc 
LOCAL_SRC_FILES         := $(subst $(LOCAL_PATH)/,,$(wildcard $(SRC)/*/*.cpp))
LOCAL_EXPORT_C_INCLUDES := $(addprefix $(SRC)/, interface)
LOCAL_C_INCLUDES        := $(subst $(LOCAL_PATH)/../,,$(wildcard $(SRC)/[^.]*/))
LOCAL_CPPFLAGS          := -Dandroid -frtti -fexceptions

include $(BUILD_SHARED_LIBRARY)

