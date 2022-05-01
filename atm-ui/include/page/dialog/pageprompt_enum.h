#ifndef PAGEPROMPT_ENUM_H
#define PAGEPROMPT_ENUM_H

// Need seperate header file for PagePrompt enums to avoid circular dependency in the pagemanager.h file
namespace PromptEnum {
    enum Icon {
        info,
        question,
        warning,
        error,
        none
    };
};

#endif // PAGEPROMPT_ENUM_H
