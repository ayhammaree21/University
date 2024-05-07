import javafx.beans.property.SimpleStringProperty;


import javafx.beans.property.StringProperty;


public class ResultTable {

	private final int PID;
    private final int number_of_fault;
    private final double start;
    private final double finish;
    private final double turnAround;
    private final double waitTime;
    
  
	public ResultTable(int idOfProcess, int fault, double timeOfArrival, double finish, double turningAroundTime,
			double waitingTime) {
		this.PID=idOfProcess;
    	this.number_of_fault= fault;
    	this.start= timeOfArrival;
    	this.finish= finish;
    	this.turnAround= turningAroundTime;
    	this.waitTime= waitingTime;
	}

	public StringProperty PIDProperty(){
		return  new SimpleStringProperty(String.valueOf(PID));
		}
    public StringProperty number_of_faultProperty(){return  new SimpleStringProperty(String.valueOf(number_of_fault));}
    public StringProperty startProperty(){return new SimpleStringProperty(String.valueOf(start));}
    public StringProperty finishProperty(){return new SimpleStringProperty(String.valueOf(finish));}
    public StringProperty turnAroundProperty(){return new SimpleStringProperty(String.valueOf(turnAround));}
    public StringProperty waitTimeProperty(){return new SimpleStringProperty(String.valueOf(waitTime));}

	
}
