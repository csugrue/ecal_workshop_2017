using UnityEngine;
using System.Collections;
using Vuforia;

public class TrackableApp : MonoBehaviour, ITrackableEventHandler {

	private TrackableBehaviour mTrackableBehaviour;

	// Use this for initialization
	void Start () {

		mTrackableBehaviour = GetComponent<TrackableBehaviour>();
    	if (mTrackableBehaviour) {
      		mTrackableBehaviour.RegisterTrackableEventHandler(this);
      		print("trackable ready");
    	}

	}
	
	public void OnTrackableStateChanged(TrackableBehaviour.Status previousStatus,TrackableBehaviour.Status newStatus){ 
	    if (newStatus == TrackableBehaviour.Status.DETECTED ||
	        newStatus == TrackableBehaviour.Status.TRACKED ||
	        newStatus == TrackableBehaviour.Status.EXTENDED_TRACKED){
	      OnTrackingFound();
	    }
  	}

  	private void OnTrackingFound(){ 
  		
  		print("Tracking Found");
  		        
  	}

}
