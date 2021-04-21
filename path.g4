grammar path;

path: (command WHITESPACE)+ command? EOF;
command: (command_type|command_type WHITESPACE points+=point+);
command_type: COMMAND_TYPE;
point: COORD (COMMA COORD)? WHITESPACE?;

COMMAND_TYPE: [a-zA-Z];

COMMA: ',';
COORD: ('-')?[0-9]+([.][0-9]+)?;
WHITESPACE  : ' ';
