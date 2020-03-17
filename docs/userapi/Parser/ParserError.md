---
layout: class
title: momiji::ParserError
in-header: <momiji/Parser.h>
declaration: struct ParserError
description: A struct representing a parsing error.

fields:
    'line':
        type: std::int64_t
        default: 0
        description: The line where the error occured.


    'errorType':
        type: ErrorType
        default: momiji::errors::UnknownError
        description: The type of the error.

    'codeStr':
        type: 'std::string'
        description: The string of the line where the error occured.

typedefs:
    'ErrorType':
        definition: |
            ```cpp
            std::variant<momiji::errors::UnknownError,
                         momiji::errors::InvalidRegisterNumber,
                         momiji::errors::NoInstructionFound,
                         momiji::errors::NoLabelFound,
                         momiji::errors::OperandTypeMismatch,
                         momiji::errors::DataTypeMismatch,
                         momiji::errors::UnexpectedCharacter,
                         momiji::errors::MissingCharacter,
                         momiji::errors::UnknownOperand,
                         momiji::errors::UnexpectedSectionContent>;
            ```
        description: A variant describing the possible types of errors.
---
