using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI; 

public class SaveAndGetData : MonoBehaviour
{
    [SerializeField] InputField field; 
    [SerializeField] UnityEngine.UI.Text textview; 
    public Material lamp; 

    public void SaveData() {
        string input = field.text; 
        PlayerPrefs.SetString("Input", input);
    }

    public void ShowData() {
        string s = PlayerPrefs.GetString("Input");
        int input = int.Parse(s);
        string result = "";
        if (input > 15 || input < 0) {
            lamp.EnableKeyword("_EMISSION");
            result = "Wrong input";
        } else if (input == 0) {
            result = "0000";
        } else {
            lamp.DisableKeyword("_EMISSION");
            while (input > 0) {
                int remainder = input%2;
                input = input/2;
                result = remainder.ToString() + result; 
            }
        }
        while (result.Length < 4) {
            result = "0" + result;
        }
        textview.text = result; 
        PlayerPrefs.SetString("Output", result);
    }
}
