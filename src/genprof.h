#ifndef __GENPROF_H__
#define __GENPROF_H__

/*
 Copyright (c) 2006 Trevor Williams

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program;
 if not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*!
 \file    genprof.h
 \author  Trevor Williams  (phase1geo@gmail.com)
 \date    12/10/2007
*/

#include "defines.h"

#define NUM_PROFILES 1119

#ifdef DEBUG
#define UNREGISTERED 0
#define FSM_ARG_PARSE_STATE 1
#define FSM_ARG_PARSE 2
#define FSM_ARG_PARSE_VALUE 3
#define FSM_ARG_PARSE_TRANS 4
#define FSM_ARG_PARSE_ATTR 5
#define SYM_VALUE_STORE 6
#define ADD_SYM_VALUES_TO_SIM 7
#define COVERED_VALUE_CHANGE_BIN 8
#define COVERED_VALUE_CHANGE_REAL 9
#define COVERED_END_OF_SIM 10
#define COVERED_CB_ERROR_HANDLER 11
#define GEN_NEXT_SYMBOL 12
#define COVERED_CREATE_VALUE_CHANGE_CB 13
#define COVERED_PARSE_TASK_FUNC 14
#define COVERED_PARSE_SIGNALS 15
#define COVERED_PARSE_INSTANCE 16
#define COVERED_SIM_CALLTF 17
#define COVERED_REGISTER 18
#define LEXER_KEYWORD_SV_CODE 19
#define VSIGNAL_INIT 20
#define VSIGNAL_CREATE 21
#define VSIGNAL_CREATE_VEC 22
#define VSIGNAL_DUPLICATE 23
#define VSIGNAL_DB_WRITE 24
#define VSIGNAL_DB_READ 25
#define VSIGNAL_DB_MERGE 26
#define VSIGNAL_MERGE 27
#define VSIGNAL_PROPAGATE 28
#define VSIGNAL_VCD_ASSIGN 29
#define VSIGNAL_ADD_EXPRESSION 30
#define VSIGNAL_FROM_STRING 31
#define VSIGNAL_CALC_WIDTH_FOR_EXPR 32
#define VSIGNAL_CALC_LSB_FOR_EXPR 33
#define VSIGNAL_DEALLOC 34
#define LEXER_KEYWORD_1995_CODE 35
#define STMT_ITER_RESET 36
#define STMT_ITER_COPY 37
#define STMT_ITER_NEXT 38
#define STMT_ITER_REVERSE 39
#define STMT_ITER_FIND_HEAD 40
#define STMT_ITER_GET_NEXT_IN_ORDER 41
#define STMT_ITER_GET_LINE_BEFORE 42
#define LEXER_CSTRING_A 43
#define LEXER_KEYWORD_A 44
#define LEXER_ESCAPED_NAME 45
#define LEXER_SYSTEM_CALL 46
#define LINE_DIRECTIVE 47
#define LINE_DIRECTIVE2 48
#define PROCESS_TIMESCALE_TOKEN 49
#define PROCESS_TIMESCALE 50
#define LEXER_YYWRAP 51
#define RESET_LEXER 52
#define RESET_LEXER_FOR_GENERATION 53
#define CHECK_FOR_PRAGMA 54
#define STATEMENT_CREATE 55
#define STATEMENT_QUEUE_ADD 56
#define STATEMENT_QUEUE_COMPARE 57
#define STATEMENT_SIZE_ELEMENTS 58
#define STATEMENT_DB_WRITE 59
#define STATEMENT_DB_WRITE_TREE 60
#define STATEMENT_DB_WRITE_EXPR_TREE 61
#define STATEMENT_DB_READ 62
#define STATEMENT_ASSIGN_EXPR_IDS 63
#define STATEMENT_CONNECT 64
#define STATEMENT_GET_LAST_LINE_HELPER 65
#define STATEMENT_GET_LAST_LINE 66
#define STATEMENT_FIND_RHS_SIGS 67
#define STATEMENT_FIND_HEAD_STATEMENT 68
#define STATEMENT_FIND_STATEMENT 69
#define STATEMENT_FIND_STATEMENT_BY_POSITION 70
#define STATEMENT_CONTAINS_EXPR_CALLING_STMT 71
#define STATEMENT_DEALLOC_RECURSIVE 72
#define STATEMENT_DEALLOC 73
#define ENUMERATE_ADD_ITEM 74
#define ENUMERATE_END_LIST 75
#define ENUMERATE_RESOLVE 76
#define ENUMERATE_DEALLOC 77
#define ENUMERATE_DEALLOC_LIST 78
#define PARSER_STATIC_EXPR_PRIMARY_A 79
#define PARSER_STATIC_EXPR_PRIMARY_B 80
#define PARSER_EXPRESSION_LIST_A 81
#define PARSER_EXPRESSION_LIST_B 82
#define PARSER_EXPRESSION_LIST_C 83
#define PARSER_EXPRESSION_LIST_D 84
#define PARSER_IDENTIFIER_A 85
#define PARSER_GENERATE_CASE_ITEM_A 86
#define PARSER_GENERATE_CASE_ITEM_B 87
#define PARSER_GENERATE_CASE_ITEM_C 88
#define PARSER_STATEMENT_BEGIN_A 89
#define PARSER_STATEMENT_FORK_A 90
#define PARSER_STATEMENT_FOR_A 91
#define PARSER_CASE_ITEM_A 92
#define PARSER_CASE_ITEM_B 93
#define PARSER_CASE_ITEM_C 94
#define PARSER_DELAY_VALUE_A 95
#define PARSER_DELAY_VALUE_B 96
#define PARSER_PARAMETER_VALUE_BYNAME_A 97
#define PARSER_GATE_INSTANCE_A 98
#define PARSER_GATE_INSTANCE_B 99
#define PARSER_GATE_INSTANCE_C 100
#define PARSER_GATE_INSTANCE_D 101
#define PARSER_LIST_OF_NAMES_A 102
#define PARSER_LIST_OF_NAMES_B 103
#define ATTRIBUTE_CREATE 104
#define ATTRIBUTE_PARSE 105
#define ATTRIBUTE_DEALLOC 106
#define SEARCH_INIT 107
#define SEARCH_ADD_INCLUDE_PATH 108
#define SEARCH_ADD_DIRECTORY_PATH 109
#define SEARCH_ADD_FILE 110
#define SEARCH_ADD_NO_SCORE_FUNIT 111
#define SEARCH_ADD_EXTENSIONS 112
#define SEARCH_FREE_LISTS 113
#define RANK_CREATE_COMP_CDD_COV 114
#define RANK_DEALLOC_COMP_CDD_COV 115
#define RANK_CHECK_INDEX 116
#define RANK_GATHER_SIGNAL_COV 117
#define RANK_GATHER_COMB_COV 118
#define RANK_GATHER_EXPRESSION_COV 119
#define RANK_GATHER_FSM_COV 120
#define RANK_CALC_NUM_CPS 121
#define RANK_GATHER_COMP_CDD_COV 122
#define RANK_READ_CDD 123
#define RANK_SELECTED_CDD_COV 124
#define RANK_PERFORM_WEIGHTED_SELECTION 125
#define RANK_PERFORM_GREEDY_SORT 126
#define RANK_COUNT_CPS 127
#define RANK_PERFORM 128
#define RANK_OUTPUT 129
#define COMMAND_RANK 130
#define SIM_CURRENT_THREAD 131
#define SIM_THREAD_POP_HEAD 132
#define SIM_THREAD_INSERT_INTO_DELAY_QUEUE 133
#define SIM_THREAD_PUSH 134
#define SIM_EXPR_CHANGED 135
#define SIM_CREATE_THREAD 136
#define SIM_ADD_THREAD 137
#define SIM_KILL_THREAD 138
#define SIM_KILL_THREAD_WITH_FUNIT 139
#define SIM_ADD_STATICS 140
#define SIM_EXPRESSION 141
#define SIM_THREAD 142
#define SIM_SIMULATE 143
#define SIM_INITIALIZE 144
#define SIM_STOP 145
#define SIM_FINISH 146
#define SIM_DEALLOC 147
#define SCOPE_FIND_FUNIT_FROM_SCOPE 148
#define SCOPE_FIND_PARAM 149
#define SCOPE_FIND_SIGNAL 150
#define SCOPE_FIND_TASK_FUNCTION_NAMEDBLOCK 151
#define SCOPE_GET_PARENT_FUNIT 152
#define SCOPE_GET_PARENT_MODULE 153
#define REPORT_PARSE_METRICS 154
#define REPORT_PARSE_ARGS 155
#define REPORT_GATHER_INSTANCE_STATS 156
#define REPORT_GATHER_FUNIT_STATS 157
#define REPORT_PRINT_HEADER 158
#define REPORT_GENERATE 159
#define REPORT_READ_CDD_AND_READY 160
#define REPORT_CLOSE_CDD 161
#define REPORT_SAVE_CDD 162
#define REPORT_FORMAT_EXCLUSION_REASON 163
#define REPORT_OUTPUT_EXCLUSION_REASON 164
#define COMMAND_REPORT 165
#define CHECK_OPTION_VALUE 166
#define IS_VARIABLE 167
#define IS_FUNC_UNIT 168
#define IS_LEGAL_FILENAME 169
#define GET_BASENAME 170
#define GET_DIRNAME 171
#define GET_ABSOLUTE_PATH 172
#define GET_RELATIVE_PATH 173
#define DIRECTORY_EXISTS 174
#define DIRECTORY_LOAD 175
#define FILE_EXISTS 176
#define UTIL_READLINE 177
#define GET_QUOTED_STRING 178
#define SUBSTITUTE_ENV_VARS 179
#define SCOPE_EXTRACT_FRONT 180
#define SCOPE_EXTRACT_BACK 181
#define SCOPE_EXTRACT_SCOPE 182
#define SCOPE_GEN_PRINTABLE 183
#define SCOPE_COMPARE 184
#define SCOPE_LOCAL 185
#define CONVERT_FILE_TO_MODULE 186
#define GET_NEXT_VFILE 187
#define GEN_SPACE 188
#define REMOVE_UNDERSCORES 189
#define GET_FUNIT_TYPE 190
#define CALC_MISS_PERCENT 191
#define READ_COMMAND_FILE 192
#define OVL_IS_ASSERTION_NAME 193
#define OVL_IS_ASSERTION_MODULE 194
#define OVL_IS_COVERAGE_POINT 195
#define OVL_ADD_ASSERTIONS_TO_NO_SCORE_LIST 196
#define OVL_GET_FUNIT_STATS 197
#define OVL_GET_COVERAGE_POINT 198
#define OVL_DISPLAY_VERBOSE 199
#define OVL_COLLECT 200
#define OVL_GET_COVERAGE 201
#define COMBINATION_CALC_DEPTH 202
#define COMBINATION_DOES_MULTI_EXP_NEED_UL 203
#define COMBINATION_MULTI_EXPR_CALC 204
#define COMBINATION_IS_EXPR_MULTI_NODE 205
#define COMBINATION_GET_TREE_STATS 206
#define COMBINATION_RESET_COUNTED_EXPRS 207
#define COMBINATION_RESET_COUNTED_EXPR_TREE 208
#define COMBINATION_GET_STATS 209
#define COMBINATION_GET_FUNIT_SUMMARY 210
#define COMBINATION_GET_INST_SUMMARY 211
#define COMBINATION_DISPLAY_INSTANCE_SUMMARY 212
#define COMBINATION_INSTANCE_SUMMARY 213
#define COMBINATION_DISPLAY_FUNIT_SUMMARY 214
#define COMBINATION_FUNIT_SUMMARY 215
#define COMBINATION_DRAW_LINE 216
#define COMBINATION_DRAW_CENTERED_LINE 217
#define COMBINATION_UNDERLINE_TREE 218
#define COMBINATION_PREP_LINE 219
#define COMBINATION_UNDERLINE 220
#define COMBINATION_UNARY 221
#define COMBINATION_EVENT 222
#define COMBINATION_TWO_VARS 223
#define COMBINATION_MULTI_VAR_EXPRS 224
#define COMBINATION_MULTI_EXPR_OUTPUT_LENGTH 225
#define COMBINATION_MULTI_EXPR_OUTPUT 226
#define COMBINATION_MULTI_VARS 227
#define COMBINATION_GET_MISSED_EXPR 228
#define COMBINATION_LIST_MISSED 229
#define COMBINATION_OUTPUT_EXPR 230
#define COMBINATION_DISPLAY_VERBOSE 231
#define COMBINATION_INSTANCE_VERBOSE 232
#define COMBINATION_FUNIT_VERBOSE 233
#define COMBINATION_COLLECT 234
#define COMBINATION_GET_EXCLUDE_LIST 235
#define COMBINATION_GET_EXPRESSION 236
#define COMBINATION_GET_COVERAGE 237
#define COMBINATION_REPORT 238
#define VCD_CALC_INDEX 239
#define VCD_PARSE_DEF_IGNORE 240
#define VCD_PARSE_DEF_VAR 241
#define VCD_PARSE_DEF_SCOPE 242
#define VCD_PARSE_DEF 243
#define VCD_PARSE_SIM_VECTOR 244
#define VCD_PARSE_SIM_REAL 245
#define VCD_PARSE_SIM_IGNORE 246
#define VCD_PARSE_SIM 247
#define VCD_PARSE 248
#define STMT_BLK_ADD_TO_REMOVE_LIST 249
#define STMT_BLK_REMOVE 250
#define STMT_BLK_SPECIFY_REMOVAL_REASON 251
#define STR_LINK_ADD 252
#define STMT_LINK_ADD_HEAD 253
#define STMT_LINK_ADD_TAIL 254
#define STMT_LINK_MERGE 255
#define EXP_LINK_ADD 256
#define SIG_LINK_ADD 257
#define FSM_LINK_ADD 258
#define FUNIT_LINK_ADD 259
#define GITEM_LINK_ADD 260
#define INST_LINK_ADD 261
#define STR_LINK_FIND 262
#define STMT_LINK_FIND 263
#define EXP_LINK_FIND 264
#define SIG_LINK_FIND 265
#define FSM_LINK_FIND 266
#define FUNIT_LINK_FIND 267
#define GITEM_LINK_FIND 268
#define INST_LINK_FIND_BY_SCOPE 269
#define INST_LINK_FIND_BY_FUNIT 270
#define STR_LINK_REMOVE 271
#define EXP_LINK_REMOVE 272
#define GITEM_LINK_REMOVE 273
#define FUNIT_LINK_REMOVE 274
#define STR_LINK_DELETE_LIST 275
#define STMT_LINK_UNLINK 276
#define STMT_LINK_DELETE_LIST 277
#define EXP_LINK_DELETE_LIST 278
#define SIG_LINK_DELETE_LIST 279
#define FSM_LINK_DELETE_LIST 280
#define FUNIT_LINK_DELETE_LIST 281
#define GITEM_LINK_DELETE_LIST 282
#define INST_LINK_DELETE_LIST 283
#define LEXER_KEYWORD_SYS_SV_CODE 284
#define PARSER_CHECK_PSTAR 285
#define PARSER_CHECK_ATTRIBUTE 286
#define PARSER_CREATE_ATTR_LIST 287
#define PARSER_CREATE_ATTR 288
#define PARSER_CREATE_TASK_DECL 289
#define PARSER_CREATE_TASK_BODY 290
#define PARSER_CREATE_FUNCTION_DECL 291
#define PARSER_CREATE_FUNCTION_BODY 292
#define PARSER_END_TASK_FUNCTION 293
#define PARSER_CREATE_PORT 294
#define PARSER_HANDLE_INLINE_PORT_ERROR 295
#define PARSER_CREATE_SIMPLE_NUMBER 296
#define PARSER_CREATE_COMPLEX_NUMBER 297
#define PARSER_APPEND_SE_PORT_LIST 298
#define PARSER_CREATE_SE_PORT_LIST 299
#define PARSER_CREATE_UNARY_SE 300
#define PARSER_CREATE_SYSCALL_SE 301
#define PARSER_CREATE_UNARY_EXP 302
#define PARSER_CREATE_BINARY_EXP 303
#define PARSER_CREATE_OP_AND_ASSIGN_EXP 304
#define PARSER_CREATE_SYSCALL_EXP 305
#define PARSER_CREATE_SYSCALL_W_PARAMS_EXP 306
#define PARSER_CREATE_OP_AND_ASSIGN_W_DIM_EXP 307
#define SCORE_GENERATE_TOP_VPI_MODULE 308
#define SCORE_GENERATE_TOP_DUMPVARS_MODULE 309
#define SCORE_GENERATE_PLI_TAB_FILE 310
#define SCORE_PARSE_DEFINE 311
#define SCORE_PARSE_ARGS 312
#define COMMAND_SCORE 313
#define MEMORY_GET_STAT 314
#define MEMORY_GET_STATS 315
#define MEMORY_GET_FUNIT_SUMMARY 316
#define MEMORY_GET_INST_SUMMARY 317
#define MEMORY_CREATE_PDIM_BIT_ARRAY 318
#define MEMORY_GET_MEM_COVERAGE 319
#define MEMORY_GET_COVERAGE 320
#define MEMORY_COLLECT 321
#define MEMORY_DISPLAY_TOGGLE_INSTANCE_SUMMARY 322
#define MEMORY_TOGGLE_INSTANCE_SUMMARY 323
#define MEMORY_DISPLAY_AE_INSTANCE_SUMMARY 324
#define MEMORY_AE_INSTANCE_SUMMARY 325
#define MEMORY_DISPLAY_TOGGLE_FUNIT_SUMMARY 326
#define MEMORY_TOGGLE_FUNIT_SUMMARY 327
#define MEMORY_DISPLAY_AE_FUNIT_SUMMARY 328
#define MEMORY_AE_FUNIT_SUMMARY 329
#define MEMORY_DISPLAY_MEMORY 330
#define MEMORY_DISPLAY_VERBOSE 331
#define MEMORY_INSTANCE_VERBOSE 332
#define MEMORY_FUNIT_VERBOSE 333
#define MEMORY_REPORT 334
#define DB_CREATE 335
#define DB_CLOSE 336
#define DB_CHECK_FOR_TOP_MODULE 337
#define DB_WRITE 338
#define DB_READ 339
#define DB_MERGE_INSTANCE_TREES 340
#define DB_MERGE_FUNITS 341
#define DB_SCALE_TO_PRECISION 342
#define DB_CREATE_UNNAMED_SCOPE 343
#define DB_IS_UNNAMED_SCOPE 344
#define DB_SET_TIMESCALE 345
#define DB_FIND_AND_SET_CURR_FUNIT 346
#define DB_GET_CURR_FUNIT 347
#define DB_GET_EXCLUSION_ID_SIZE 348
#define DB_GEN_EXCLUSION_ID 349
#define DB_ADD_FILE_VERSION 350
#define DB_OUTPUT_DUMPVARS 351
#define DB_ADD_INSTANCE 352
#define DB_ADD_MODULE 353
#define DB_END_MODULE 354
#define DB_ADD_FUNCTION_TASK_NAMEDBLOCK 355
#define DB_END_FUNCTION_TASK_NAMEDBLOCK 356
#define DB_ADD_DECLARED_PARAM 357
#define DB_ADD_OVERRIDE_PARAM 358
#define DB_ADD_VECTOR_PARAM 359
#define DB_ADD_DEFPARAM 360
#define DB_ADD_SIGNAL 361
#define DB_ADD_ENUM 362
#define DB_END_ENUM_LIST 363
#define DB_ADD_TYPEDEF 364
#define DB_FIND_SIGNAL 365
#define DB_ADD_GEN_ITEM_BLOCK 366
#define DB_FIND_GEN_ITEM 367
#define DB_FIND_TYPEDEF 368
#define DB_GET_CURR_GEN_BLOCK 369
#define DB_CURR_SIGNAL_COUNT 370
#define DB_CREATE_EXPRESSION 371
#define DB_BIND_EXPR_TREE 372
#define DB_CREATE_EXPR_FROM_STATIC 373
#define DB_ADD_EXPRESSION 374
#define DB_CREATE_SENSITIVITY_LIST 375
#define DB_PARALLELIZE_STATEMENT 376
#define DB_CREATE_STATEMENT 377
#define DB_ADD_STATEMENT 378
#define DB_REMOVE_STATEMENT_FROM_CURRENT_FUNIT 379
#define DB_REMOVE_STATEMENT 380
#define DB_CONNECT_STATEMENT_TRUE 381
#define DB_CONNECT_STATEMENT_FALSE 382
#define DB_GEN_ITEM_CONNECT_TRUE 383
#define DB_GEN_ITEM_CONNECT_FALSE 384
#define DB_GEN_ITEM_CONNECT 385
#define DB_STATEMENT_CONNECT 386
#define DB_CREATE_ATTR_PARAM 387
#define DB_PARSE_ATTRIBUTE 388
#define DB_REMOVE_STMT_BLKS_CALLING_STATEMENT 389
#define DB_GEN_CURR_INST_SCOPE 390
#define DB_SYNC_CURR_INSTANCE 391
#define DB_SET_VCD_SCOPE 392
#define DB_VCD_UPSCOPE 393
#define DB_ASSIGN_SYMBOL 394
#define DB_SET_SYMBOL_CHAR 395
#define DB_SET_SYMBOL_STRING 396
#define DB_DO_TIMESTEP 397
#define FSM_CREATE 398
#define FSM_ADD_ARC 399
#define FSM_CREATE_TABLES 400
#define FSM_DB_WRITE 401
#define FSM_DB_READ 402
#define FSM_DB_MERGE 403
#define FSM_MERGE 404
#define FSM_TABLE_SET 405
#define FSM_VCD_ASSIGN 406
#define FSM_GET_STATS 407
#define FSM_GET_FUNIT_SUMMARY 408
#define FSM_GET_INST_SUMMARY 409
#define FSM_GATHER_SIGNALS 410
#define FSM_COLLECT 411
#define FSM_GET_COVERAGE 412
#define FSM_DISPLAY_INSTANCE_SUMMARY 413
#define FSM_INSTANCE_SUMMARY 414
#define FSM_DISPLAY_FUNIT_SUMMARY 415
#define FSM_FUNIT_SUMMARY 416
#define FSM_DISPLAY_STATE_VERBOSE 417
#define FSM_DISPLAY_ARC_VERBOSE 418
#define FSM_DISPLAY_VERBOSE 419
#define FSM_INSTANCE_VERBOSE 420
#define FSM_FUNIT_VERBOSE 421
#define FSM_REPORT 422
#define FSM_DEALLOC 423
#define CODEGEN_CREATE_EXPR_HELPER 424
#define CODEGEN_CREATE_EXPR 425
#define CODEGEN_GEN_EXPR 426
#define CODEGEN_GEN_EXPR_ONE_LINE 427
#define TOGGLE_GET_STATS 428
#define TOGGLE_COLLECT 429
#define TOGGLE_GET_COVERAGE 430
#define TOGGLE_GET_FUNIT_SUMMARY 431
#define TOGGLE_GET_INST_SUMMARY 432
#define TOGGLE_DISPLAY_INSTANCE_SUMMARY 433
#define TOGGLE_INSTANCE_SUMMARY 434
#define TOGGLE_DISPLAY_FUNIT_SUMMARY 435
#define TOGGLE_FUNIT_SUMMARY 436
#define TOGGLE_DISPLAY_VERBOSE 437
#define TOGGLE_INSTANCE_VERBOSE 438
#define TOGGLE_FUNIT_VERBOSE 439
#define TOGGLE_REPORT 440
#define VLERROR 441
#define VLWARN 442
#define PARSER_DEALLOC_SIG_RANGE 443
#define PARSER_COPY_CURR_RANGE 444
#define PARSER_COPY_RANGE_TO_CURR_RANGE 445
#define PARSER_EXPLICITLY_SET_CURR_RANGE 446
#define PARSER_IMPLICITLY_SET_CURR_RANGE 447
#define PARSER_CHECK_GENERATION 448
#define LEXER_KEYWORD_2001_CODE 449
#define STRUCT_UNION_LENGTH 450
#define STRUCT_UNION_ADD_MEMBER 451
#define STRUCT_UNION_ADD_MEMBER_VOID 452
#define STRUCT_UNION_ADD_MEMBER_SIG 453
#define STRUCT_UNION_ADD_MEMBER_TYPEDEF 454
#define STRUCT_UNION_ADD_MEMBER_ENUM 455
#define STRUCT_UNION_ADD_MEMBER_STRUCT_UNION 456
#define STRUCT_UNION_CREATE 457
#define STRUCT_UNION_MEMBER_DEALLOC 458
#define STRUCT_UNION_DEALLOC 459
#define STRUCT_UNION_DEALLOC_LIST 460
#define BIND_ADD 461
#define BIND_APPEND_FSM_EXPR 462
#define BIND_REMOVE 463
#define BIND_FIND_SIG_NAME 464
#define BIND_PARAM 465
#define BIND_SIGNAL 466
#define BIND_TASK_FUNCTION_PORTS 467
#define BIND_TASK_FUNCTION_NAMEDBLOCK 468
#define BIND_PERFORM 469
#define BIND_DEALLOC 470
#define PERF_GEN_STATS 471
#define PERF_OUTPUT_MOD_STATS 472
#define PERF_OUTPUT_INST_REPORT_HELPER 473
#define PERF_OUTPUT_INST_REPORT 474
#define MOD_PARM_FIND 475
#define MOD_PARM_FIND_EXPR_AND_REMOVE 476
#define MOD_PARM_ADD 477
#define INST_PARM_FIND 478
#define INST_PARM_ADD 479
#define INST_PARM_ADD_GENVAR 480
#define INST_PARM_BIND 481
#define DEFPARAM_ADD 482
#define DEFPARAM_DEALLOC 483
#define PARAM_FIND_AND_SET_EXPR_VALUE 484
#define PARAM_SET_SIG_SIZE 485
#define PARAM_SIZE_FUNCTION 486
#define PARAM_EXPR_EVAL 487
#define PARAM_HAS_OVERRIDE 488
#define PARAM_HAS_DEFPARAM 489
#define PARAM_RESOLVE_DECLARED 490
#define PARAM_RESOLVE_OVERRIDE 491
#define PARAM_RESOLVE 492
#define PARAM_DB_WRITE 493
#define MOD_PARM_DEALLOC 494
#define INST_PARM_DEALLOC 495
#define SYS_TASK_UNIFORM 496
#define SYS_TASK_RTL_DIST_UNIFORM 497
#define SYS_TASK_SRANDOM 498
#define SYS_TASK_RANDOM 499
#define SYS_TASK_URANDOM 500
#define SYS_TASK_URANDOM_RANGE 501
#define SYS_TASK_REALTOBITS 502
#define SYS_TASK_BITSTOREAL 503
#define SYS_TASK_SHORTREALTOBITS 504
#define SYS_TASK_BITSTOSHORTREAL 505
#define SYS_TASK_ITOR 506
#define SYS_TASK_RTOI 507
#define SYS_TASK_STORE_PLUSARGS 508
#define SYS_TASK_TEST_PLUSARG 509
#define SYS_TASK_VALUE_PLUSARGS 510
#define SYS_TASK_DEALLOC 511
#define GENERATOR_DISPLAY 512
#define GENERATOR_SORT_FUNIT_BY_FILENAME 513
#define GENERATOR_SET_NEXT_FUNIT 514
#define GENERATOR_DEALLOC_FNAME_LIST 515
#define GENERATOR_OUTPUT_FUNIT 516
#define GENERATOR_OUTPUT 517
#define GENERATOR_INIT_FUNIT 518
#define GENERATOR_PREPEND_TO_WORK_CODE 519
#define GENERATOR_ADD_TO_WORK_CODE 520
#define GENERATOR_FLUSH_WORK_CODE1 521
#define GENERATOR_ADD_TO_HOLD_CODE 522
#define GENERATOR_FLUSH_HOLD_CODE1 523
#define GENERATOR_FLUSH_EVENT_COMB 524
#define GENERATOR_FLUSH_EVENT_COMBS1 525
#define GENERATOR_FLUSH_ALL1 526
#define GENERATOR_FIND_STATEMENT 527
#define GENERATOR_FIND_CASE_STATEMENT 528
#define GENERATOR_INSERT_LINE_COV_WITH_STMT 529
#define GENERATOR_INSERT_LINE_COV 530
#define GENERATOR_INSERT_MEM_COV 531
#define GENERATOR_INSERT_EVENT_COMB_COV 532
#define GENERATOR_INSERT_UNARY_COMB_COV 533
#define GENERATOR_INSERT_AND_COMB_COV 534
#define GENERATOR_GEN_MSB 535
#define GENERATOR_CREATE_RHS 536
#define GENERATOR_CREATE_SUBEXP 537
#define GENERATOR_CONCAT_CODE 538
#define GENERATOR_CREATE_EXP 539
#define GENERATOR_INSERT_SUBEXP 540
#define GENERATOR_INSERT_COMB_COV_HELPER 541
#define GENERATOR_INSERT_MEM_COV_HELPER 542
#define GENERATOR_INSERT_COMB_COV 543
#define GENERATOR_INSERT_COMB_COV_FROM_STMT_STACK 544
#define GENERATOR_INSERT_COMB_COV_WITH_STMT 545
#define GENERATOR_INSERT_CASE_COMB_COV 546
#define GENERATOR_INSERT_FSM_COVS 547
#define DEF_LOOKUP 548
#define IS_DEFINED 549
#define DEF_MATCH 550
#define DEF_START 551
#define DEFINE_MACRO 552
#define DO_DEFINE 553
#define DEF_IS_DONE 554
#define DEF_FINISH 555
#define DEF_UNDEFINE 556
#define INCLUDE_FILENAME 557
#define DO_INCLUDE 558
#define YYWRAP 559
#define RESET_PPLEXER 560
#define REENTRANT_COUNT_AFU_BITS 561
#define REENTRANT_STORE_DATA_BITS 562
#define REENTRANT_RESTORE_DATA_BITS 563
#define REENTRANT_CREATE 564
#define REENTRANT_DEALLOC 565
#define PARSE_READLINE 566
#define PARSE_DESIGN 567
#define PARSE_AND_SCORE_DUMPFILE 568
#define LEXER_KEYWORD_SYS_2001_CODE 569
#define OBFUSCATE_SET_MODE 570
#define OBFUSCATE_NAME 571
#define OBFUSCATE_DEALLOC 572
#define SCORE_ADD_ARGS 573
#define INFO_SET_VECTOR_ELEM_SIZE 574
#define INFO_DB_WRITE 575
#define INFO_DB_READ 576
#define ARGS_DB_READ 577
#define MESSAGE_DB_READ 578
#define MERGED_CDD_DB_READ 579
#define INFO_DEALLOC 580
#define ASSERTION_PARSE 581
#define ASSERTION_PARSE_ATTR 582
#define ASSERTION_GET_STATS 583
#define ASSERTION_DISPLAY_INSTANCE_SUMMARY 584
#define ASSERTION_INSTANCE_SUMMARY 585
#define ASSERTION_DISPLAY_FUNIT_SUMMARY 586
#define ASSERTION_FUNIT_SUMMARY 587
#define ASSERTION_DISPLAY_VERBOSE 588
#define ASSERTION_INSTANCE_VERBOSE 589
#define ASSERTION_FUNIT_VERBOSE 590
#define ASSERTION_REPORT 591
#define ASSERTION_GET_FUNIT_SUMMARY 592
#define ASSERTION_COLLECT 593
#define ASSERTION_GET_COVERAGE 594
#define LXT2_RD_EXPAND_INTEGER_TO_BITS 595
#define LXT2_RD_EXPAND_BITS_TO_INTEGER 596
#define LXT2_RD_ITER_RADIX 597
#define LXT2_RD_ITER_RADIX0 598
#define LXT2_RD_BUILD_RADIX 599
#define LXT2_RD_REGENERATE_PROCESS_MASK 600
#define LXT2_RD_PROCESS_BLOCK 601
#define LXT2_RD_INIT 602
#define LXT2_RD_CLOSE 603
#define LXT2_RD_GET_FACNAME 604
#define LXT2_RD_ITER_BLOCKS 605
#define LXT2_RD_LIMIT_TIME_RANGE 606
#define LXT2_RD_UNLIMIT_TIME_RANGE 607
#define TREE_ADD 608
#define TREE_FIND 609
#define TREE_REMOVE 610
#define TREE_DEALLOC 611
#define EXPRESSION_CREATE_TMP_VECS 612
#define EXPRESSION_CREATE_VALUE 613
#define EXPRESSION_CREATE 614
#define EXPRESSION_SET_VALUE 615
#define EXPRESSION_SET_SIGNED 616
#define EXPRESSION_RESIZE 617
#define EXPRESSION_GET_ID 618
#define EXPRESSION_GET_FIRST_LINE_EXPR 619
#define EXPRESSION_GET_LAST_LINE_EXPR 620
#define EXPRESSION_GET_CURR_DIMENSION 621
#define EXPRESSION_FIND_RHS_SIGS 622
#define EXPRESSION_FIND_PARAMS 623
#define EXPRESSION_FIND_ULINE_ID 624
#define EXPRESSION_FIND_EXPR 625
#define EXPRESSION_CONTAINS_EXPR_CALLING_STMT 626
#define EXPRESSION_GET_ROOT_STATEMENT 627
#define EXPRESSION_ASSIGN_EXPR_IDS 628
#define EXPRESSION_DB_WRITE 629
#define EXPRESSION_DB_WRITE_TREE 630
#define EXPRESSION_DB_READ 631
#define EXPRESSION_DB_MERGE 632
#define EXPRESSION_MERGE 633
#define EXPRESSION_STRING_OP 634
#define EXPRESSION_STRING 635
#define EXPRESSION_OP_FUNC__XOR 636
#define EXPRESSION_OP_FUNC__XOR_A 637
#define EXPRESSION_OP_FUNC__MULTIPLY 638
#define EXPRESSION_OP_FUNC__MULTIPLY_A 639
#define EXPRESSION_OP_FUNC__DIVIDE 640
#define EXPRESSION_OP_FUNC__DIVIDE_A 641
#define EXPRESSION_OP_FUNC__MOD 642
#define EXPRESSION_OP_FUNC__MOD_A 643
#define EXPRESSION_OP_FUNC__ADD 644
#define EXPRESSION_OP_FUNC__ADD_A 645
#define EXPRESSION_OP_FUNC__SUBTRACT 646
#define EXPRESSION_OP_FUNC__SUB_A 647
#define EXPRESSION_OP_FUNC__AND 648
#define EXPRESSION_OP_FUNC__AND_A 649
#define EXPRESSION_OP_FUNC__OR 650
#define EXPRESSION_OP_FUNC__OR_A 651
#define EXPRESSION_OP_FUNC__NAND 652
#define EXPRESSION_OP_FUNC__NOR 653
#define EXPRESSION_OP_FUNC__NXOR 654
#define EXPRESSION_OP_FUNC__LT 655
#define EXPRESSION_OP_FUNC__GT 656
#define EXPRESSION_OP_FUNC__LSHIFT 657
#define EXPRESSION_OP_FUNC__LSHIFT_A 658
#define EXPRESSION_OP_FUNC__RSHIFT 659
#define EXPRESSION_OP_FUNC__RSHIFT_A 660
#define EXPRESSION_OP_FUNC__ARSHIFT 661
#define EXPRESSION_OP_FUNC__ARSHIFT_A 662
#define EXPRESSION_OP_FUNC__TIME 663
#define EXPRESSION_OP_FUNC__RANDOM 664
#define EXPRESSION_OP_FUNC__SASSIGN 665
#define EXPRESSION_OP_FUNC__SRANDOM 666
#define EXPRESSION_OP_FUNC__URANDOM 667
#define EXPRESSION_OP_FUNC__URANDOM_RANGE 668
#define EXPRESSION_OP_FUNC__REALTOBITS 669
#define EXPRESSION_OP_FUNC__BITSTOREAL 670
#define EXPRESSION_OP_FUNC__SHORTREALTOBITS 671
#define EXPRESSION_OP_FUNC__BITSTOSHORTREAL 672
#define EXPRESSION_OP_FUNC__ITOR 673
#define EXPRESSION_OP_FUNC__RTOI 674
#define EXPRESSION_OP_FUNC__TEST_PLUSARGS 675
#define EXPRESSION_OP_FUNC__VALUE_PLUSARGS 676
#define EXPRESSION_OP_FUNC__EQ 677
#define EXPRESSION_OP_FUNC__CEQ 678
#define EXPRESSION_OP_FUNC__LE 679
#define EXPRESSION_OP_FUNC__GE 680
#define EXPRESSION_OP_FUNC__NE 681
#define EXPRESSION_OP_FUNC__CNE 682
#define EXPRESSION_OP_FUNC__LOR 683
#define EXPRESSION_OP_FUNC__LAND 684
#define EXPRESSION_OP_FUNC__COND 685
#define EXPRESSION_OP_FUNC__COND_SEL 686
#define EXPRESSION_OP_FUNC__UINV 687
#define EXPRESSION_OP_FUNC__UAND 688
#define EXPRESSION_OP_FUNC__UNOT 689
#define EXPRESSION_OP_FUNC__UOR 690
#define EXPRESSION_OP_FUNC__UXOR 691
#define EXPRESSION_OP_FUNC__UNAND 692
#define EXPRESSION_OP_FUNC__UNOR 693
#define EXPRESSION_OP_FUNC__UNXOR 694
#define EXPRESSION_OP_FUNC__NULL 695
#define EXPRESSION_OP_FUNC__SIG 696
#define EXPRESSION_OP_FUNC__SBIT 697
#define EXPRESSION_OP_FUNC__MBIT 698
#define EXPRESSION_OP_FUNC__EXPAND 699
#define EXPRESSION_OP_FUNC__LIST 700
#define EXPRESSION_OP_FUNC__CONCAT 701
#define EXPRESSION_OP_FUNC__PEDGE 702
#define EXPRESSION_OP_FUNC__NEDGE 703
#define EXPRESSION_OP_FUNC__AEDGE 704
#define EXPRESSION_OP_FUNC__EOR 705
#define EXPRESSION_OP_FUNC__SLIST 706
#define EXPRESSION_OP_FUNC__DELAY 707
#define EXPRESSION_OP_FUNC__TRIGGER 708
#define EXPRESSION_OP_FUNC__CASE 709
#define EXPRESSION_OP_FUNC__CASEX 710
#define EXPRESSION_OP_FUNC__CASEZ 711
#define EXPRESSION_OP_FUNC__DEFAULT 712
#define EXPRESSION_OP_FUNC__BASSIGN 713
#define EXPRESSION_OP_FUNC__FUNC_CALL 714
#define EXPRESSION_OP_FUNC__TASK_CALL 715
#define EXPRESSION_OP_FUNC__NB_CALL 716
#define EXPRESSION_OP_FUNC__FORK 717
#define EXPRESSION_OP_FUNC__JOIN 718
#define EXPRESSION_OP_FUNC__DISABLE 719
#define EXPRESSION_OP_FUNC__REPEAT 720
#define EXPRESSION_OP_FUNC__EXPONENT 721
#define EXPRESSION_OP_FUNC__PASSIGN 722
#define EXPRESSION_OP_FUNC__MBIT_POS 723
#define EXPRESSION_OP_FUNC__MBIT_NEG 724
#define EXPRESSION_OP_FUNC__NEGATE 725
#define EXPRESSION_OP_FUNC__IINC 726
#define EXPRESSION_OP_FUNC__PINC 727
#define EXPRESSION_OP_FUNC__IDEC 728
#define EXPRESSION_OP_FUNC__PDEC 729
#define EXPRESSION_OP_FUNC__DLY_ASSIGN 730
#define EXPRESSION_OP_FUNC__DLY_OP 731
#define EXPRESSION_OP_FUNC__REPEAT_DLY 732
#define EXPRESSION_OP_FUNC__DIM 733
#define EXPRESSION_OP_FUNC__WAIT 734
#define EXPRESSION_OP_FUNC__FINISH 735
#define EXPRESSION_OP_FUNC__STOP 736
#define EXPRESSION_OPERATE 737
#define EXPRESSION_OPERATE_RECURSIVELY 738
#define EXPRESSION_VCD_ASSIGN 739
#define EXPRESSION_IS_STATIC_ONLY_HELPER 740
#define EXPRESSION_IS_ASSIGNED 741
#define EXPRESSION_IS_BIT_SELECT 742
#define EXPRESSION_IS_LAST_SELECT 743
#define EXPRESSION_IS_IN_RASSIGN 744
#define EXPRESSION_SET_ASSIGNED 745
#define EXPRESSION_SET_CHANGED 746
#define EXPRESSION_ASSIGN 747
#define EXPRESSION_DEALLOC 748
#define GEN_ITEM_STRINGIFY 749
#define GEN_ITEM_DISPLAY 750
#define GEN_ITEM_DISPLAY_BLOCK_HELPER 751
#define GEN_ITEM_DISPLAY_BLOCK 752
#define GEN_ITEM_COMPARE 753
#define GEN_ITEM_FIND 754
#define GEN_ITEM_REMOVE_IF_CONTAINS_EXPR_CALLING_STMT 755
#define GEN_ITEM_GET_GENVAR 756
#define GEN_ITEM_VARNAME_CONTAINS_GENVAR 757
#define GEN_ITEM_CALC_SIGNAL_NAME 758
#define GEN_ITEM_CREATE_EXPR 759
#define GEN_ITEM_CREATE_SIG 760
#define GEN_ITEM_CREATE_STMT 761
#define GEN_ITEM_CREATE_INST 762
#define GEN_ITEM_CREATE_TFN 763
#define GEN_ITEM_CREATE_BIND 764
#define GEN_ITEM_RESIZE_STMTS_AND_SIGS 765
#define GEN_ITEM_ASSIGN_IDS 766
#define GEN_ITEM_DB_WRITE 767
#define GEN_ITEM_DB_WRITE_EXPR_TREE 768
#define GEN_ITEM_CONNECT 769
#define GEN_ITEM_RESOLVE 770
#define GEN_ITEM_BIND 771
#define GENERATE_RESOLVE 772
#define GENERATE_REMOVE_STMT_HELPER 773
#define GENERATE_REMOVE_STMT 774
#define GEN_ITEM_DEALLOC 775
#define FSM_VAR_ADD 776
#define FSM_VAR_IS_OUTPUT_STATE 777
#define FSM_VAR_BIND_EXPR 778
#define FSM_VAR_ADD_EXPR 779
#define FSM_VAR_BIND_STMT 780
#define FSM_VAR_BIND_ADD 781
#define FSM_VAR_STMT_ADD 782
#define FSM_VAR_BIND 783
#define FSM_VAR_DEALLOC 784
#define FSM_VAR_REMOVE 785
#define FSM_VAR_CLEANUP 786
#define INSTANCE_DISPLAY_TREE_HELPER 787
#define INSTANCE_DISPLAY_TREE 788
#define INSTANCE_CREATE 789
#define INSTANCE_GEN_SCOPE 790
#define INSTANCE_COMPARE 791
#define INSTANCE_FIND_SCOPE 792
#define INSTANCE_FIND_BY_FUNIT 793
#define INSTANCE_FIND_BY_FUNIT_NAME_IF_ONE_HELPER 794
#define INSTANCE_FIND_BY_FUNIT_NAME_IF_ONE 795
#define INSTANCE_FIND_SIGNAL_BY_EXCLUSION_ID 796
#define INSTANCE_FIND_EXPRESSION_BY_EXCLUSION_ID 797
#define INSTANCE_FIND_FSM_ARC_INDEX_BY_EXCLUSION_ID 798
#define INSTANCE_ADD_CHILD 799
#define INSTANCE_COPY 800
#define INSTANCE_PARSE_ADD 801
#define INSTANCE_RESOLVE_INST 802
#define INSTANCE_RESOLVE_HELPER 803
#define INSTANCE_RESOLVE 804
#define INSTANCE_READ_ADD 805
#define INSTANCE_MERGE 806
#define INSTANCE_GET_LEADING_HIERARCHY 807
#define INSTANCE_MARK_LHIER_DIFFS 808
#define INSTANCE_MERGE_TWO_TREES 809
#define INSTANCE_DB_WRITE 810
#define INSTANCE_ONLY_DB_READ 811
#define INSTANCE_ONLY_DB_MERGE 812
#define INSTANCE_REMOVE_STMT_BLKS_CALLING_STMT 813
#define INSTANCE_REMOVE_PARMS_WITH_EXPR 814
#define INSTANCE_DEALLOC_SINGLE 815
#define INSTANCE_OUTPUT_DUMPVARS 816
#define INSTANCE_DEALLOC_TREE 817
#define INSTANCE_DEALLOC 818
#define VECTOR_INIT 819
#define VECTOR_INT_R64 820
#define VECTOR_INT_R32 821
#define VECTOR_CREATE 822
#define VECTOR_COPY 823
#define VECTOR_COPY_RANGE 824
#define VECTOR_CLONE 825
#define VECTOR_DB_WRITE 826
#define VECTOR_DB_READ 827
#define VECTOR_DB_MERGE 828
#define VECTOR_MERGE 829
#define VECTOR_GET_EVAL_A 830
#define VECTOR_GET_EVAL_B 831
#define VECTOR_GET_EVAL_C 832
#define VECTOR_GET_EVAL_D 833
#define VECTOR_GET_EVAL_AB_COUNT 834
#define VECTOR_GET_EVAL_ABC_COUNT 835
#define VECTOR_GET_EVAL_ABCD_COUNT 836
#define VECTOR_GET_TOGGLE01_ULONG 837
#define VECTOR_GET_TOGGLE10_ULONG 838
#define VECTOR_DISPLAY_TOGGLE01_ULONG 839
#define VECTOR_DISPLAY_TOGGLE10_ULONG 840
#define VECTOR_TOGGLE_COUNT 841
#define VECTOR_MEM_RW_COUNT 842
#define VECTOR_SET_ASSIGNED 843
#define VECTOR_SET_COVERAGE_AND_ASSIGN 844
#define VECTOR_GET_SIGN_EXTEND_VECTOR_ULONG 845
#define VECTOR_SIGN_EXTEND_ULONG 846
#define VECTOR_LSHIFT_ULONG 847
#define VECTOR_RSHIFT_ULONG 848
#define VECTOR_SET_VALUE 849
#define VECTOR_PART_SELECT_PULL 850
#define VECTOR_PART_SELECT_PUSH 851
#define VECTOR_SET_UNARY_EVALS 852
#define VECTOR_SET_AND_COMB_EVALS 853
#define VECTOR_SET_OR_COMB_EVALS 854
#define VECTOR_SET_OTHER_COMB_EVALS 855
#define VECTOR_IS_UKNOWN 856
#define VECTOR_IS_NOT_ZERO 857
#define VECTOR_SET_TO_X 858
#define VECTOR_TO_INT 859
#define VECTOR_TO_UINT64 860
#define VECTOR_TO_REAL64 861
#define VECTOR_TO_SIM_TIME 862
#define VECTOR_FROM_INT 863
#define VECTOR_FROM_UINT64 864
#define VECTOR_FROM_REAL64 865
#define VECTOR_SET_STATIC 866
#define VECTOR_TO_STRING 867
#define VECTOR_FROM_STRING_FIXED 868
#define VECTOR_FROM_STRING 869
#define VECTOR_VCD_ASSIGN 870
#define VECTOR_VCD_ASSIGN2 871
#define VECTOR_BITWISE_AND_OP 872
#define VECTOR_BITWISE_NAND_OP 873
#define VECTOR_BITWISE_OR_OP 874
#define VECTOR_BITWISE_NOR_OP 875
#define VECTOR_BITWISE_XOR_OP 876
#define VECTOR_BITWISE_NXOR_OP 877
#define VECTOR_OP_LT 878
#define VECTOR_OP_LE 879
#define VECTOR_OP_GT 880
#define VECTOR_OP_GE 881
#define VECTOR_OP_EQ 882
#define VECTOR_CEQ_ULONG 883
#define VECTOR_OP_CEQ 884
#define VECTOR_OP_CXEQ 885
#define VECTOR_OP_CZEQ 886
#define VECTOR_OP_NE 887
#define VECTOR_OP_CNE 888
#define VECTOR_OP_LOR 889
#define VECTOR_OP_LAND 890
#define VECTOR_OP_LSHIFT 891
#define VECTOR_OP_RSHIFT 892
#define VECTOR_OP_ARSHIFT 893
#define VECTOR_OP_ADD 894
#define VECTOR_OP_NEGATE 895
#define VECTOR_OP_SUBTRACT 896
#define VECTOR_OP_MULTIPLY 897
#define VECTOR_OP_DIVIDE 898
#define VECTOR_OP_MODULUS 899
#define VECTOR_OP_INC 900
#define VECTOR_OP_DEC 901
#define VECTOR_UNARY_INV 902
#define VECTOR_UNARY_AND 903
#define VECTOR_UNARY_NAND 904
#define VECTOR_UNARY_OR 905
#define VECTOR_UNARY_NOR 906
#define VECTOR_UNARY_XOR 907
#define VECTOR_UNARY_NXOR 908
#define VECTOR_UNARY_NOT 909
#define VECTOR_OP_EXPAND 910
#define VECTOR_OP_LIST 911
#define VECTOR_DEALLOC_VALUE 912
#define VECTOR_DEALLOC 913
#define SYMTABLE_ADD_SYM_SIG 914
#define SYMTABLE_ADD_SYM_EXP 915
#define SYMTABLE_ADD_SYM_FSM 916
#define SYMTABLE_INIT 917
#define SYMTABLE_CREATE 918
#define SYMTABLE_GET_TABLE 919
#define SYMTABLE_ADD_SIGNAL 920
#define SYMTABLE_ADD_EXPRESSION 921
#define SYMTABLE_ADD_FSM 922
#define SYMTABLE_SET_VALUE 923
#define SYMTABLE_ASSIGN 924
#define SYMTABLE_DEALLOC 925
#define FUNIT_INIT 926
#define FUNIT_CREATE 927
#define FUNIT_GET_CURR_MODULE 928
#define FUNIT_GET_CURR_MODULE_SAFE 929
#define FUNIT_GET_CURR_FUNCTION 930
#define FUNIT_GET_CURR_TASK 931
#define FUNIT_GET_PORT_COUNT 932
#define FUNIT_FIND_PARAM 933
#define FUNIT_FIND_SIGNAL 934
#define FUNIT_REMOVE_STMT_BLKS_CALLING_STMT 935
#define FUNIT_GEN_TASK_FUNCTION_NAMEDBLOCK_NAME 936
#define FUNIT_SIZE_ELEMENTS 937
#define FUNIT_DB_WRITE 938
#define FUNIT_DB_READ 939
#define FUNIT_VERSION_DB_READ 940
#define FUNIT_DB_MERGE 941
#define FUNIT_MERGE 942
#define FUNIT_FLATTEN_NAME 943
#define FUNIT_FIND_BY_ID 944
#define FUNIT_IS_TOP_MODULE 945
#define FUNIT_IS_UNNAMED 946
#define FUNIT_IS_UNNAMED_CHILD_OF 947
#define FUNIT_IS_CHILD_OF 948
#define FUNIT_DISPLAY_SIGNALS 949
#define FUNIT_DISPLAY_EXPRESSIONS 950
#define STATEMENT_ADD_THREAD 951
#define FUNIT_PUSH_THREADS 952
#define STATEMENT_DELETE_THREAD 953
#define FUNIT_OUTPUT_DUMPVARS 954
#define FUNIT_CLEAN 955
#define FUNIT_DEALLOC 956
#define ARC_FIND_FROM_STATE 957
#define ARC_FIND_TO_STATE 958
#define ARC_FIND_ARC 959
#define ARC_FIND_ARC_BY_EXCLUSION_ID 960
#define ARC_CREATE 961
#define ARC_ADD 962
#define ARC_STATE_HITS 963
#define ARC_TRANSITION_HITS 964
#define ARC_TRANSITION_EXCLUDED 965
#define ARC_GET_STATS 966
#define ARC_DB_WRITE 967
#define ARC_DB_READ 968
#define ARC_DB_MERGE 969
#define ARC_MERGE 970
#define ARC_GET_STATES 971
#define ARC_GET_TRANSITIONS 972
#define ARC_ARE_ANY_EXCLUDED 973
#define ARC_DEALLOC 974
#define LEXER_KEYWORD_SYS_1995_CODE 975
#define FUNC_ITER_DISPLAY 976
#define FUNC_ITER_SORT 977
#define FUNC_ITER_COUNT_STMT_ITERS 978
#define FUNC_ITER_ADD_STMT_ITERS 979
#define FUNC_ITER_ADD_SIG_LINKS 980
#define FUNC_ITER_INIT 981
#define FUNC_ITER_RESET 982
#define FUNC_ITER_GET_NEXT_STATEMENT 983
#define FUNC_ITER_GET_NEXT_SIGNAL 984
#define FUNC_ITER_DEALLOC 985
#define EXCLUDE_EXPR_ASSIGN_AND_RECALC 986
#define EXCLUDE_SIG_ASSIGN_AND_RECALC 987
#define EXCLUDE_ARC_ASSIGN_AND_RECALC 988
#define EXCLUDE_ADD_EXCLUDE_REASON 989
#define EXCLUDE_REMOVE_EXCLUDE_REASON 990
#define EXCLUDE_IS_LINE_EXCLUDED 991
#define EXCLUDE_SET_LINE_EXCLUDE 992
#define EXCLUDE_IS_TOGGLE_EXCLUDED 993
#define EXCLUDE_SET_TOGGLE_EXCLUDE 994
#define EXCLUDE_IS_COMB_EXCLUDED 995
#define EXCLUDE_SET_COMB_EXCLUDE 996
#define EXCLUDE_IS_FSM_EXCLUDED 997
#define EXCLUDE_SET_FSM_EXCLUDE 998
#define EXCLUDE_IS_ASSERT_EXCLUDED 999
#define EXCLUDE_SET_ASSERT_EXCLUDE 1000
#define EXCLUDE_FIND_EXCLUDE_REASON 1001
#define EXCLUDE_DB_WRITE 1002
#define EXCLUDE_DB_READ 1003
#define EXCLUDE_RESOLVE_REASON 1004
#define EXCLUDE_DB_MERGE 1005
#define EXCLUDE_MERGE 1006
#define EXCLUDE_FIND_SIGNAL 1007
#define EXCLUDE_FIND_EXPRESSION 1008
#define EXCLUDE_FIND_FSM_ARC 1009
#define EXCLUDE_FORMAT_REASON 1010
#define EXCLUDED_GET_MESSAGE 1011
#define EXCLUDE_HANDLE_EXCLUDE_REASON 1012
#define EXCLUDE_PRINT_EXCLUSION 1013
#define EXCLUDE_LINE_FROM_ID 1014
#define EXCLUDE_TOGGLE_FROM_ID 1015
#define EXCLUDE_MEMORY_FROM_ID 1016
#define EXCLUDE_EXPR_FROM_ID 1017
#define EXCLUDE_FSM_FROM_ID 1018
#define EXCLUDE_ASSERT_FROM_ID 1019
#define EXCLUDE_APPLY_EXCLUSIONS 1020
#define COMMAND_EXCLUDE 1021
#define RACE_BLK_CREATE 1022
#define RACE_FIND_HEAD_STATEMENT_CONTAINING_STATEMENT_HELPER 1023
#define RACE_FIND_HEAD_STATEMENT_CONTAINING_STATEMENT 1024
#define RACE_GET_HEAD_STATEMENT 1025
#define RACE_FIND_HEAD_STATEMENT 1026
#define RACE_CALC_STMT_BLK_TYPE 1027
#define RACE_CALC_EXPR_ASSIGNMENT 1028
#define RACE_CALC_ASSIGNMENTS 1029
#define RACE_HANDLE_RACE_CONDITION 1030
#define RACE_CHECK_ASSIGNMENT_TYPES 1031
#define RACE_CHECK_ONE_BLOCK_ASSIGNMENT 1032
#define RACE_CHECK_RACE_COUNT 1033
#define RACE_CHECK_MODULES 1034
#define RACE_DB_WRITE 1035
#define RACE_DB_READ 1036
#define RACE_GET_STATS 1037
#define RACE_REPORT_SUMMARY 1038
#define RACE_REPORT_VERBOSE 1039
#define RACE_REPORT 1040
#define RACE_COLLECT_LINES 1041
#define RACE_BLK_DELETE_LIST 1042
#define STATISTIC_CREATE 1043
#define STATISTIC_IS_EMPTY 1044
#define STATISTIC_DEALLOC 1045
#define TCL_FUNC_GET_RACE_REASON_MSGS 1046
#define TCL_FUNC_GET_FUNIT_LIST 1047
#define TCL_FUNC_GET_INSTANCES 1048
#define TCL_FUNC_GET_INSTANCE_LIST 1049
#define TCL_FUNC_IS_FUNIT 1050
#define TCL_FUNC_GET_FUNIT 1051
#define TCL_FUNC_GET_INST 1052
#define TCL_FUNC_GET_FUNIT_NAME 1053
#define TCL_FUNC_GET_FILENAME 1054
#define TCL_FUNC_INST_SCOPE 1055
#define TCL_FUNC_GET_FUNIT_START_AND_END 1056
#define TCL_FUNC_COLLECT_UNCOVERED_LINES 1057
#define TCL_FUNC_COLLECT_COVERED_LINES 1058
#define TCL_FUNC_COLLECT_RACE_LINES 1059
#define TCL_FUNC_COLLECT_UNCOVERED_TOGGLES 1060
#define TCL_FUNC_COLLECT_COVERED_TOGGLES 1061
#define TCL_FUNC_COLLECT_UNCOVERED_MEMORIES 1062
#define TCL_FUNC_COLLECT_COVERED_MEMORIES 1063
#define TCL_FUNC_GET_TOGGLE_COVERAGE 1064
#define TCL_FUNC_GET_MEMORY_COVERAGE 1065
#define TCL_FUNC_COLLECT_UNCOVERED_COMBS 1066
#define TCL_FUNC_COLLECT_COVERED_COMBS 1067
#define TCL_FUNC_GET_COMB_EXPRESSION 1068
#define TCL_FUNC_GET_COMB_COVERAGE 1069
#define TCL_FUNC_COLLECT_UNCOVERED_FSMS 1070
#define TCL_FUNC_COLLECT_COVERED_FSMS 1071
#define TCL_FUNC_GET_FSM_COVERAGE 1072
#define TCL_FUNC_COLLECT_UNCOVERED_ASSERTIONS 1073
#define TCL_FUNC_COLLECT_COVERED_ASSERTIONS 1074
#define TCL_FUNC_GET_ASSERT_COVERAGE 1075
#define TCL_FUNC_OPEN_CDD 1076
#define TCL_FUNC_CLOSE_CDD 1077
#define TCL_FUNC_SAVE_CDD 1078
#define TCL_FUNC_MERGE_CDD 1079
#define TCL_FUNC_GET_LINE_SUMMARY 1080
#define TCL_FUNC_GET_TOGGLE_SUMMARY 1081
#define TCL_FUNC_GET_MEMORY_SUMMARY 1082
#define TCL_FUNC_GET_COMB_SUMMARY 1083
#define TCL_FUNC_GET_FSM_SUMMARY 1084
#define TCL_FUNC_GET_ASSERT_SUMMARY 1085
#define TCL_FUNC_PREPROCESS_VERILOG 1086
#define TCL_FUNC_GET_SCORE_PATH 1087
#define TCL_FUNC_GET_INCLUDE_PATHNAME 1088
#define TCL_FUNC_GET_GENERATION 1089
#define TCL_FUNC_SET_LINE_EXCLUDE 1090
#define TCL_FUNC_SET_TOGGLE_EXCLUDE 1091
#define TCL_FUNC_SET_MEMORY_EXCLUDE 1092
#define TCL_FUNC_SET_COMB_EXCLUDE 1093
#define TCL_FUNC_FSM_EXCLUDE 1094
#define TCL_FUNC_SET_ASSERT_EXCLUDE 1095
#define TCL_FUNC_GENERATE_REPORT 1096
#define TCL_FUNC_INITIALIZE 1097
#define STATIC_EXPR_GEN_UNARY 1098
#define STATIC_EXPR_GEN 1099
#define STATIC_EXPR_CALC_LSB_AND_WIDTH_PRE 1100
#define STATIC_EXPR_CALC_LSB_AND_WIDTH_POST 1101
#define STATIC_EXPR_DEALLOC 1102
#define VCDID 1103
#define VCD_CALLBACK 1104
#define LXT_PARSE 1105
#define COMMAND_MERGE 1106
#define LINE_GET_STATS 1107
#define LINE_COLLECT 1108
#define LINE_GET_FUNIT_SUMMARY 1109
#define LINE_GET_INST_SUMMARY 1110
#define LINE_DISPLAY_INSTANCE_SUMMARY 1111
#define LINE_INSTANCE_SUMMARY 1112
#define LINE_DISPLAY_FUNIT_SUMMARY 1113
#define LINE_FUNIT_SUMMARY 1114
#define LINE_DISPLAY_VERBOSE 1115
#define LINE_INSTANCE_VERBOSE 1116
#define LINE_FUNIT_VERBOSE 1117
#define LINE_REPORT 1118

extern profiler profiles[NUM_PROFILES];
#endif

extern unsigned int profile_index;

#endif

