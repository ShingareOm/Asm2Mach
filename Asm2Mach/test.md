Let’s break down your project into three main phases: **Fetching Instructions**, **Converting Instructions**, and **Displaying Output**.

### Phase 1: Fetching Instructions
- **Goal**: Read the assembly file and process labels (e.g., `START:`) and instructions (e.g., `MOV AX, 5`).
- **Functions**: 
  - `processLabels()`: Reads the file line by line, extracts labels, and stores them with memory addresses. It also enqueues non-label instructions into a queue.
  - `removeComments()`: Strips out comments in the assembly code (anything after `;`).

**Example**:  
Given the line `START: MOV AX, 5`, `processLabels()` stores `START` with an address, and the instruction `MOV AX, 5` is added to the queue for further processing.

### Phase 2: Converting Instructions
- **Goal**: Convert assembly instructions (like `MOV AX, 5`) to their machine code (opcode).
- **Functions**:
  - `fetchOpcode()`: Given an instruction’s mnemonic (like `MOV`), it fetches the corresponding opcode (like `89`).
  - `convertInstructionToOpcode()`: Parses an instruction and converts it into machine-readable opcode, considering operands and addressing modes.
  - `resolveLabel()`: Resolves labels into actual memory addresses by looking them up in the symbol table.

**Example**:  
For the instruction `MOV AX, 5`, `fetchOpcode()` returns `89`. If the instruction has a label (e.g., `JMP START`), `resolveLabel()` is used to replace the label with the corresponding memory address of `START`.

### Phase 3: Displaying Output
- **Goal**: Display the instruction queue and data segment in a formatted way.
- **Functions**:
  - `displayQueue()`: Prints out the instructions currently in the queue.
  - `displayDataSegment()`: Displays all the data (e.g., `DB`, `DW`, `RESB`, `RESW`) processed in the data segment, showing reserved memory or defined values.

**Example**:  
After processing the instructions, the output might show something like:
```
Instruction Queue:
MOV AX, 5
ADD AX, 10
JMP END
```
The data segment will show values defined by directives like `DB` or `DW` (e.g., `DB 0x01, 0x02`).

### Key Functions & Data Structures
- **InstructionQueue**: Stores instructions in a queue for processing (FIFO order).
- **SymbolTable**: Stores labels and their corresponding memory addresses.
- **OpcodeTable**: A table of known mnemonics and their corresponding opcodes (e.g., `MOV` to `89`).
- **Data Segment**: Holds values for variables, constants, and reserved memory.

### Why This Structure and Approach
1. **Queues** are used to store instructions in a sequential manner for easy processing.
2. **SymbolTable** enables label resolution, which is crucial in assembly for addressing.
3. **OpcodeTable** provides an easy mapping between mnemonics and machine code, making instruction translation efficient.
4. **Complexity**:  
   - **Time complexity**: Mostly linear (O(n)) for most operations, as instructions are processed sequentially.
   - **Space complexity**: Dependent on the size of the instruction queue, symbol table, and data segment.

### Counter-arguments & Why Not Other Structures
- **Why not use arrays instead of queues?**  
  A queue provides an inherent ordering mechanism (FIFO), ensuring that instructions are processed in the correct sequence.
  
- **Why not use a hash table for labels?**  
  A symbol table (array of structures) is simpler and faster for small to medium-sized projects compared to hash tables, which add unnecessary complexity.

### Conclusion
This project reads assembly code, processes labels, converts instructions to machine code, and displays the result. Each phase ensures that assembly code is correctly parsed, translated, and displayed for further execution.