package com.example.ai_project;

public class Course {
    private static int count = 0;
    public int id;
    public String courseCode;
    public char courseType;
    public String courseName;
    public String instructorName;
    public String section;
   public TimeSlot timeSlot;

    public Course( String courseCode,char courseType,String section,String instructorName,String courseName){
        this.id=count;
        this.courseType=courseType;
        this.courseCode=courseCode;
        this.courseName=courseName;
        this.instructorName=instructorName;
        this.section=section;
        count++;
    }


}
