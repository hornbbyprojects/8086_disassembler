void parse_mov_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_MOV;
u8 literal_1 = ((current_byte >> 2) &0b111111);if (0b100010 != literal_1) {ret -> failed = 1; return;}
ret -> flag_d = ((current_byte >> 1) &0b1);
ret -> flag_w = ((current_byte >> 0) &0b1);
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> mod = ((current_byte >> 6) &0b11);
ret -> reg = ((current_byte >> 3) &0b111);
ret -> r_m = ((current_byte >> 0) &0b111);
read_displacement(file, ret);if (feof(file)) { ret -> failed = 1; return; }
}

void parse_mov_1(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_MOV;
u8 literal_1 = ((current_byte >> 1) &0b1111111);if (0b1100011 != literal_1) {ret -> failed = 1; return;}
ret -> flag_w = ((current_byte >> 0) &0b1);
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> mod = ((current_byte >> 6) &0b11);
u8 literal_2 = ((current_byte >> 3) &0b111);if (0b000 != literal_2) {ret -> failed = 1; return;}
ret -> r_m = ((current_byte >> 0) &0b111);
read_displacement(file, ret);if (feof(file)) { ret -> failed = 1; return; }
read_data(file, ret);if (feof(file)) { ret -> failed = 1; return; }
}

void parse_mov_2(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_MOV;
u8 literal_1 = ((current_byte >> 4) &0b1111);if (0b1011 != literal_1) {ret -> failed = 1; return;}
ret -> flag_w = ((current_byte >> 3) &0b1);
ret -> reg = ((current_byte >> 0) &0b111);
read_data(file, ret);if (feof(file)) { ret -> failed = 1; return; }
}

void parse_mov_3(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_MOV;
u8 literal_1 = ((current_byte >> 2) &0b111111);if (0b101000 != literal_1) {ret -> failed = 1; return;}
ret -> flag_d = ((current_byte >> 1) &0b1);
ret -> flag_d = !ret -> flag_d;
ret -> flag_w = ((current_byte >> 0) &0b1);
ret -> mod = 0;
ret -> reg = 0;
ret -> r_m = 0b110;
read_displacement(file, ret);if (feof(file)) { ret -> failed = 1; return; }
}

void parse_add_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_ADD;
u8 literal_1 = ((current_byte >> 6) &0b11);if (0b00 != literal_1) {ret -> failed = 1; return;}
u8 literal_2 = ((current_byte >> 3) &0b111);if (0b000 != literal_2) {ret -> failed = 1; return;}
u8 literal_3 = ((current_byte >> 2) &0b1);if (0b0 != literal_3) {ret -> failed = 1; return;}
ret -> flag_d = ((current_byte >> 1) &0b1);
ret -> flag_w = ((current_byte >> 0) &0b1);
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> mod = ((current_byte >> 6) &0b11);
ret -> reg = ((current_byte >> 3) &0b111);
ret -> r_m = ((current_byte >> 0) &0b111);
read_displacement(file, ret);if (feof(file)) { ret -> failed = 1; return; }
}

void parse_add_1(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_ADD;
u8 literal_1 = ((current_byte >> 2) &0b111111);if (0b100000 != literal_1) {ret -> failed = 1; return;}
ret -> flag_s = ((current_byte >> 1) &0b1);
ret -> flag_w = ((current_byte >> 0) &0b1);
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> mod = ((current_byte >> 6) &0b11);
u8 literal_2 = ((current_byte >> 3) &0b111);if (0b000 != literal_2) {ret -> failed = 1; return;}
ret -> r_m = ((current_byte >> 0) &0b111);
read_displacement(file, ret);if (feof(file)) { ret -> failed = 1; return; }
read_data(file, ret);if (feof(file)) { ret -> failed = 1; return; }
}

void parse_add_2(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_ADD;
u8 literal_1 = ((current_byte >> 6) &0b11);if (0b00 != literal_1) {ret -> failed = 1; return;}
u8 literal_2 = ((current_byte >> 3) &0b111);if (0b000 != literal_2) {ret -> failed = 1; return;}
u8 literal_3 = ((current_byte >> 1) &0b11);if (0b10 != literal_3) {ret -> failed = 1; return;}
ret -> flag_w = ((current_byte >> 0) &0b1);
read_data(file, ret);if (feof(file)) { ret -> failed = 1; return; }
ret -> reg = 0;
}

void parse_sub_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_SUB;
u8 literal_1 = ((current_byte >> 6) &0b11);if (0b00 != literal_1) {ret -> failed = 1; return;}
u8 literal_2 = ((current_byte >> 3) &0b111);if (0b101 != literal_2) {ret -> failed = 1; return;}
u8 literal_3 = ((current_byte >> 2) &0b1);if (0b0 != literal_3) {ret -> failed = 1; return;}
ret -> flag_d = ((current_byte >> 1) &0b1);
ret -> flag_w = ((current_byte >> 0) &0b1);
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> mod = ((current_byte >> 6) &0b11);
ret -> reg = ((current_byte >> 3) &0b111);
ret -> r_m = ((current_byte >> 0) &0b111);
read_displacement(file, ret);if (feof(file)) { ret -> failed = 1; return; }
}

void parse_sub_1(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_SUB;
u8 literal_1 = ((current_byte >> 2) &0b111111);if (0b100000 != literal_1) {ret -> failed = 1; return;}
ret -> flag_s = ((current_byte >> 1) &0b1);
ret -> flag_w = ((current_byte >> 0) &0b1);
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> mod = ((current_byte >> 6) &0b11);
u8 literal_2 = ((current_byte >> 3) &0b111);if (0b101 != literal_2) {ret -> failed = 1; return;}
ret -> r_m = ((current_byte >> 0) &0b111);
read_displacement(file, ret);if (feof(file)) { ret -> failed = 1; return; }
read_data(file, ret);if (feof(file)) { ret -> failed = 1; return; }
}

void parse_sub_2(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_SUB;
u8 literal_1 = ((current_byte >> 6) &0b11);if (0b00 != literal_1) {ret -> failed = 1; return;}
u8 literal_2 = ((current_byte >> 3) &0b111);if (0b101 != literal_2) {ret -> failed = 1; return;}
u8 literal_3 = ((current_byte >> 1) &0b11);if (0b10 != literal_3) {ret -> failed = 1; return;}
ret -> flag_w = ((current_byte >> 0) &0b1);
read_data(file, ret);if (feof(file)) { ret -> failed = 1; return; }
ret -> reg = 0;
}

void parse_cmp_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_CMP;
u8 literal_1 = ((current_byte >> 6) &0b11);if (0b00 != literal_1) {ret -> failed = 1; return;}
u8 literal_2 = ((current_byte >> 3) &0b111);if (0b111 != literal_2) {ret -> failed = 1; return;}
u8 literal_3 = ((current_byte >> 2) &0b1);if (0b0 != literal_3) {ret -> failed = 1; return;}
ret -> flag_d = ((current_byte >> 1) &0b1);
ret -> flag_w = ((current_byte >> 0) &0b1);
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> mod = ((current_byte >> 6) &0b11);
ret -> reg = ((current_byte >> 3) &0b111);
ret -> r_m = ((current_byte >> 0) &0b111);
read_displacement(file, ret);if (feof(file)) { ret -> failed = 1; return; }
}

void parse_cmp_1(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_CMP;
u8 literal_1 = ((current_byte >> 2) &0b111111);if (0b100000 != literal_1) {ret -> failed = 1; return;}
ret -> flag_s = ((current_byte >> 1) &0b1);
ret -> flag_w = ((current_byte >> 0) &0b1);
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> mod = ((current_byte >> 6) &0b11);
u8 literal_2 = ((current_byte >> 3) &0b111);if (0b111 != literal_2) {ret -> failed = 1; return;}
ret -> r_m = ((current_byte >> 0) &0b111);
read_displacement(file, ret);if (feof(file)) { ret -> failed = 1; return; }
read_data(file, ret);if (feof(file)) { ret -> failed = 1; return; }
}

void parse_cmp_2(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_CMP;
u8 literal_1 = ((current_byte >> 6) &0b11);if (0b00 != literal_1) {ret -> failed = 1; return;}
u8 literal_2 = ((current_byte >> 3) &0b111);if (0b111 != literal_2) {ret -> failed = 1; return;}
u8 literal_3 = ((current_byte >> 1) &0b11);if (0b10 != literal_3) {ret -> failed = 1; return;}
ret -> flag_w = ((current_byte >> 0) &0b1);
read_data(file, ret);if (feof(file)) { ret -> failed = 1; return; }
ret -> reg = 0;
}

void parse_JE_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JE;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01110100 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JL_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JL;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01111100 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JLE_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JLE;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01111110 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JB_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JB;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01110010 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JBE_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JBE;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01110110 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JP_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JP;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01111010 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JO_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JO;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01110000 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JS_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JS;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01111000 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JNE_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JNE;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01110101 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JNL_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JNL;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01111101 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JG_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JG;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01111111 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JAE_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JAE;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01110011 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JA_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JA;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01110111 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JNP_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JNP;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01111011 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JNO_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JNO;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01110001 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JNS_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JNS;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b01111001 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_LOOP_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_LOOP;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b11100010 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_LOOPE_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_LOOPE;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b11100001 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_LOOPNE_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_LOOPNE;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b11100000 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void parse_JCXZ_0(struct InstructionData *ret, FILE *file) {
u8 current_byte = read_byte(file);
if (feof(file)) { fprintf(stderr, "EOF AT START OF instruction"); ; exit(-2); }
ret -> instruction_type = INSTRUCTION_TYPE_JCXZ;
u8 literal_1 = ((current_byte >> 0) &0b11111111);if (0b11100011 != literal_1) {ret -> failed = 1; return;}
current_byte = read_byte(file);
if (feof(file)) { ret -> failed = 1; return; }
ret -> jump_destination = current_byte;ret -> has_jump_destination = 1;
}

void (*parse_methods[]) (struct InstructionData *, FILE *) = {
parse_mov_0,
parse_mov_1,
parse_mov_2,
parse_mov_3,
parse_add_0,
parse_add_1,
parse_add_2,
parse_sub_0,
parse_sub_1,
parse_sub_2,
parse_cmp_0,
parse_cmp_1,
parse_cmp_2,
parse_JE_0,
parse_JL_0,
parse_JLE_0,
parse_JB_0,
parse_JBE_0,
parse_JP_0,
parse_JO_0,
parse_JS_0,
parse_JNE_0,
parse_JNL_0,
parse_JG_0,
parse_JAE_0,
parse_JA_0,
parse_JNP_0,
parse_JNO_0,
parse_JNS_0,
parse_LOOP_0,
parse_LOOPE_0,
parse_LOOPNE_0,
parse_JCXZ_0,
};
