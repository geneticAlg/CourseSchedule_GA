# CourseSchedule_GA
This project aims to schedule the courses of ECE department in Ohio State University automatically given the following constraints and information related to the offered courses:

# Basic constraints:

1)	The same instructor can’t teach 2 different courses simultaneously.
2)	The same room can’t be scheduled for 2 different courses simultaneously.
3)	The room scheduled for a specific course must have no less seats than the corresponding course capacity.

# Advanced constraints:

1)	For the courses with course number in the range of 3000~5999, the 3rd digit of the course number represents its study area (e.g. 0-Comm/DSP, 1-Emag, 2-Circuits, 3-SSEP, 4-Power, 5-Control, 6-Computer). The courses with the same study area can’t be scheduled in the same time slots. 
2)	For the courses with course number in the range of 5000~8999, the 3rd digit of the course number represents its study area (e.g. 0-Comm/DSP, 1-Emag, 2-Circuits, 3-SSEP, 4-Power, 5-Control, 6-Computer). The courses with the same study area can’t be scheduled in the same time slots.
3)	Some instructors may have preference of time to teach their courses (e.g. Professor Lee may only have time to teach the course on 10AM-MWF).
4)	No more than 2 large volume courses can be scheduled at the same time.
5)	No more than 1 extra-large volume course can be scheduled at the same time.

# Information related to the offered courses:

1)	Course: the course is classified by the OSU course ID system, e.g. 5421. Usually the course ID is represented as 4-digit number, the first digit represents course difficulty, the third digit represent course field.
2)	Class: a class is a real scheduling object we are dealing with, a course can have multiple classes, taught by different or same instructor.
3)	Time Slot: all the available time a course can choose
4)	Instructor: a class need an instructor to teach, sometimes we don't know who will be teaching the course, some time we know the instructor will be teaching multiple course, some time the instructor requires some preferred teaching time, we need to consider all the situation
5)	Classroom: a classroom is a physical container of the class, we need to know the classroom capacity, classroom type (e.g. Lecture or Lab, does it include computers, etc.)

# Current process of this project:
All basic and advanced constraints have been implemented. Currently, we are working on testing these features, and optimize the program with multithreading.
