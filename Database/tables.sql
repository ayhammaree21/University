drop database studyin;
create database StudyIn ;
use studyin ;
create table University(
                           Uneversity_name varchar(32),
                           image blob,
                           primary Key (Uneversity_name)
);
create table School_levels(
                              image blob,
                              Level_describtion varchar(32),
                              primary Key (Level_describtion)
);
create table Users(
                      profile_image blob,
                      User_Name varchar(32) primary Key,
                      User_Type varchar(10),
                      Email varchar(50),
                      Date_Of_Birth date,
                      Pass varchar(32),
                      Gender varchar(6),
                      Phone_Number varchar(15),
                      TheName varchar(100),
                      AccountCreationDate date,
                      allowPosting boolean default true,
                      Uneversity_name varchar(32),
                      Level_describtion varchar(32),
                      foreign key (Uneversity_name) REFERENCES University(Uneversity_name) on update cascade on delete set null ,
                      foreign key (Level_describtion) REFERENCES School_levels(Level_describtion) on update cascade on delete set null

);

create table college(

                        college_id integer auto_increment,
                        college_name varchar(32),
                        image blob,
                        Uneversity_name varchar(32),
                        primary Key (college_id),
                        foreign key (Uneversity_name) REFERENCES University(Uneversity_name) on update cascade on delete cascade

);

create table major(
                      major_id integer auto_increment,
                      major_name varchar(32),
                      image blob,
                      college_id integer,
                      primary Key (major_id),
                      foreign key (college_id) REFERENCES college(college_id) on update cascade on delete cascade

);

create table UnivCourse(
                           UnivCourse_ID varchar(10),
                           UnivCourse_name varchar(32),
                           primary Key (UnivCourse_ID)
);

create table course_to_major(
                                UnivCourse_ID varchar(10),
                                major_id integer,
                                primary Key (UnivCourse_ID,major_id),
                                foreign key (UnivCourse_ID) REFERENCES UnivCourse(UnivCourse_Id) on update cascade on delete cascade,
                                foreign key (major_id) REFERENCES major(major_id) on update cascade on delete cascade
);

CREATE TABLE school_courses(
                               course_name varchar(32),
                               image blob,
                               Course_semister varchar(2),
                               Level_describtion varchar(32),
                               PRIMARY KEY (course_name,Level_describtion),
                               FOREIGN KEY (Level_describtion) REFERENCES school_levels(Level_describtion) on update cascade on delete cascade
);

create table sources(
                        puplishedDate date,
                        numberOfUsersLike integer default 0 ,
                        LikeOrNot boolean,
                        sourceName varchar(32),
                        User_Name varchar(32),
                        school_course_name varchar(32),
                        UnivCourse_ID varchar(10),
                        sourceFile LONGBLOB,
                        primary key(sourceName,User_Name),
                        FOREIGN KEY (User_Name) REFERENCES Users(User_Name) on update cascade ,
                        FOREIGN KEY (school_course_name) REFERENCES school_courses(course_name) on update cascade,
                        FOREIGN KEY (UnivCourse_ID) REFERENCES UnivCourse(UnivCourse_ID) on update cascade

);
create table Likes(
                      Id integer auto_increment,
                      sourceName  varchar(32),
                      User_Name varchar(32),
                      primary key(Id),
                      FOREIGN KEY (User_Name) REFERENCES Users(User_Name) on update cascade on delete cascade ,
                      FOREIGN KEY (sourceName) REFERENCES sources(sourceName) on update cascade on delete cascade

);
select * from sources ;
drop table likes ;
drop table sources ;