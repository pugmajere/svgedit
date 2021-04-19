grammar path;

path: (COMMAND POINTS)* EOF;
COMMAND: [a-zA-Z];
POINTS: POINT+;
POINT: COORD ',' COORD;
COORD: ('-')?[0-9]+;
