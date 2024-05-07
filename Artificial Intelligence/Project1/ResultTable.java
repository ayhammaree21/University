package com.example.ai_project;

public class ResultTable {

    private final String courseCode;
    private final String courseName;
    private final String instructorName;
    private final String section;
    private final String days;
    private final String time;


    public ResultTable(String courseCode, String courseName, String instructorName, String section, String days, String time) {
        this.courseCode = courseCode;
        this.courseName = courseName;
        this.instructorName = instructorName;
        this.section = section;
        this.days = days;
        this.time = time;
    }

    public String getCourseCode() {
        return courseCode;
    }

    public String getCourseName() {
        return courseName;
    }

    public String getInstructorName() {
        return instructorName;
    }

    public String getSection() {
        return section;
    }

    public String getDays() {
        return days;
    }

    public String getTime() {
        return time;
    }
}
