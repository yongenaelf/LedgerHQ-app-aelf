#include "instruction.h"
#include "serum_assert_owner_instruction.h"
#include "sol/parser.h"
#include "sol/message.h"
#include "sol/print_config.h"
#include "spl_associated_token_account_instruction.h"
#include "spl_token_instruction.h"
#include "system_instruction.h"
#include "stake_instruction.h"
#include "vote_instruction.h"
#include "transaction_printers.h"
#include "util.h"
#include <string.h>

// change this if you want to be able to add succesive tx
#define MAX_INSTRUCTIONS 1

int process_message_body(const uint8_t* message_body,
                         int message_body_length,
                         const PrintConfig* print_config) {
    const MessageHeader* header = &print_config->header;
    PRINTF("GUILANE %d\n", header->instructions_length);
    // BAIL_IF(header->instructions_length == 0);
    // BAIL_IF(header->instructions_length > MAX_INSTRUCTIONS);

    size_t instruction_count = 0;
    InstructionInfo instruction_info[MAX_INSTRUCTIONS];
    explicit_bzero(instruction_info, sizeof(InstructionInfo) * MAX_INSTRUCTIONS);
    size_t display_instruction_count = 0;
    InstructionInfo* display_instruction_info[MAX_INSTRUCTIONS];

    Parser parser = {message_body, message_body_length};
    // for (; instruction_count < header->instructions_length; instruction_count++) {
        Instruction instruction;
        PRINTF("GUILANE3 %d\n", parser.buffer_length);
        // BAIL_IF(parse_instruction(&parser, &instruction));
        PRINTF("GUILANE2\n");
        // BAIL_IF(instruction_validate(&instruction, header));

        InstructionInfo* info = &instruction_info[instruction_count];
        PRINTF("GUILANE22\n");
        parse_system_transfer_instruction(&parser, &instruction, header, &info->system.transfer);
        switch (info->kind) {
            case ProgramIdSplAssociatedTokenAccount:
            case ProgramIdSplToken:
            case ProgramIdSystem:
            case ProgramIdStake:
            case ProgramIdVote:
            case ProgramIdUnknown:
                display_instruction_info[display_instruction_count++] = info;
                break;
            // Ignored instructions
            case ProgramIdSerumAssertOwner:
            case ProgramIdSplMemo:
                break;
        }
    // }
    PRINTF("GUILANE1 %d\n", header->versioned);
    // if (header->versioned) {
    //     size_t account_tables_length;
    //     BAIL_IF(parse_length(&parser, &account_tables_length));
    //     BAIL_IF(account_tables_length > 0);
    // }
    PRINTF("GUILANEA\n");
    // Ensure we've consumed the entire message body
    BAIL_IF(!parser_is_empty(&parser));
    PRINTF("GUILANEB\n");
    // If we don't know about all of the instructions, bail
    for (size_t i = 0; i < instruction_count; i++) {
        BAIL_IF(instruction_info[i].kind == ProgramIdUnknown);
    }
    PRINTF("GUILANEC\n");
    // return print_transaction(print_config, display_instruction_info, display_instruction_count);
    // return print_system_info(&display_instruction_info[0]->system, print_config);
    return print_system_transfer_info(&display_instruction_info[0]->system.transfer, print_config);
}
