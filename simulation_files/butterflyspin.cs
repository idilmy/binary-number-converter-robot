using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class butterflyspin : MonoBehaviour
{
    public Image icon1;
    public Text text;

    // Update is called once per frame
    void Update()
    {

        if(icon1.GetComponent<Image>().enabled == true){  // replace keycode with correct number printed bool
        text.text = "Done! Restart?"; 
        StartCoroutine(rotate());

        }
        
    }
    
    IEnumerator rotate() { 
        
        float time = 1.2f; 

        while(time > 0) { 

            transform.Rotate(0f, 1f * Time.deltaTime, 0f, Space.Self);
            time -= Time.deltaTime;

            yield return null;

        }

}
}


//0f, 150f * Time.deltaTime, 0f, Space.Self