using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Detection : MonoBehaviour
{
    public HingeJoint hj = new HingeJoint();
    public Material lamp; 
    public Text Gathered;
    public int position = 3; 
    public string binary = ""; 
    public bool b; 
    public bool w;
   

    public void Start() {
        if (PlayerPrefs.HasKey("Output")) {
            binary = PlayerPrefs.GetString("Output"); 
            position = 3;
            b = false;
            w = false;
        } else {
            Debug.Log("No input detected");
        }
    }

    void OnTriggerEnter(Collider col) {
        if (col.gameObject.name.Contains("disk")) {
           // hj.useLimits = false;
            if (col.gameObject.name.Contains("black")) {
                b = true; 
                Debug.Log("black"); 
                SelectionDisk();
            } else if (col.gameObject.name.Contains("white")) {
                w = true;
                Debug.Log("white"); 
                SelectionDisk();
            } else {
                b = false;
                w = false;
                //TURN ON THE LED HERE
                lamp.EnableKeyword("_EMISSION");
                Debug.Log("ignore");
            }
        }
    }

    void OnTriggerExit() {
        hj.useLimits = true;
        lamp.DisableKeyword("_EMISSION");
    }

    void SelectionDisk() {
        if (position < 0) {
            Debug.Log("Done!");
            binary = ""; 
            b = false; 
            w = false; 
            Gathered.text = "Done! Restart?";
            return; 
        } 

        //Enable the "Disks Gathered" text
        Debug.Log(binary[position]);
        if (binary[position].Equals('1') && b) {
            hj.useLimits = false;
            position = position -1;
            b = false; 
        } else if (binary[position].Equals('0') && w) {
            hj.useLimits = false;
            position = position -1;
            w = false; 
        } else {
            w = false;
            b = false;
            return;
        }

        if (position == 2) {
            Gathered.enabled = true;
        }
        
    }

}

