---
layout: class
title: momiji::errors::DataTypeMismatch
in-header: <momiji/Parser.h>
description: |
    Error triggered when an instruction doesn't support the provided data type.
declaration: struct DataTypeMismatch

fields:
    'acceptedDataTypes':
        type: std::vector<momiji::DataType>
        description: Vector of accepted data types for that instruction.

    'inputDataType':
        type: momiji::DataType
        description: The provided data type for that instruction.
---
