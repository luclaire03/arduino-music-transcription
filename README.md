# Arduino Music Transcription
An Arduino script for drawing realistic music notation on an LCD screen - staffs, whole to eighth notes, and sharps. This script draws in landscape orientation. 

# draw_note
Parameters: note (char array), duration in milliseconds (int), bpm (int) // 
Example: draw_note("F#4", 1000, 60);

draw_note first checks if there is a # in note, then parses the # away if there is. Then, draw_note uses the remaining char array to determine the appropriate location to draw the note. Currently, draw_note only supports notes from C4 to F5.

The function then uses duration and bpm to determine what type of note should be drawn. Note types supported are whole, half, dotted half, quarter, dotted quarter, and eighth. If the duration does not perfectly match to the lengths of any supported note types, the note is classified as the largest type that does not exceed its duration, and a leftover amount of time is calculated. draw_note will then be called again with leftover passed in for duration.
 
If there was a sharp in the original note array, draw_sharp is called.

# draw_staff
Parameters: x_offset (int) // 
Example: draw_staff(60);

draw_staff draws 5 parallel lines across the screen. x_offset determines how far down from the edge that these lines should be drawn (for example, drawing another staff on the screen after one has already been used). 

# draw_sharp
Parameters: center_x (int), center_y (int) // 
Example: draw_sharp(67, 10);

draw_sharp draws 4 lines around center_x, center_y, to draw a sharp.

# About
This script was originally created in Spring 2021 as part of my final project for MIT's 6.08: Interconnected and Embedded Systems class. It functions as part of a larger system that records audio, determines notes from that audio, and transcribes the notes onto an LCD screen. I have obtained written permission from course instructors to publish this code online.
