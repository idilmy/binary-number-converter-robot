using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class StartAndReset : MonoBehaviour
{
    [SerializeField] private Detection Detection;
    [SerializeField] public InputField field;
    [SerializeField] public Text preview; 
    [SerializeField] public Text Gathered;
    [SerializeField] private wall Wall; 
 
    public void onClickStart() {
    Detection.enabled = true;
    Detection.Start(); 
 }
    public void onClickReset() {
    Detection.enabled = false; 
    field.text = ""; 
    preview.text = "";
    Detection.binary = ""; 
    Detection.position = 3;
    Gathered.enabled = false;
    Gathered.text = "Disks Gathered:";
    Wall.Clear();
 }
}
