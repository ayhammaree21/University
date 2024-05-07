import javafx.beans.property.SimpleStringProperty;


import javafx.beans.property.StringProperty;


public class MemoryTable {

	
	
	    private final String process_ID;

	    private final String page_ID;

	    private final boolean trueFalse;

		public MemoryTable(String process_ID, String page_ID, boolean trueFalse) {
			
			this.process_ID = process_ID;
			this.page_ID = page_ID;
			this.trueFalse = trueFalse;
		}
	    
		  public StringProperty process_IDProperty(){return  new SimpleStringProperty(String.valueOf(process_ID));}
		    public StringProperty page_IDProperty(){return new SimpleStringProperty(String.valueOf(page_ID));}
		    public StringProperty trueFalseProperty(){return new SimpleStringProperty(String.valueOf(trueFalse));}
	    
	    
}
