using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class wall : MonoBehaviour
{
    public Image icon1;
    public Image icon2;
    public Image icon3;
    public Image icon4; 
    public Sprite black;
    public Sprite white;
    public int count =1;
    public Image holder;


    void Selection()
    {
        if (count == 1) {
            holder = icon4;
        } else if (count == 2) {
            holder = icon3;
        } else if (count == 3) {
            holder = icon2;
        } else if (count == 4) {
            holder = icon1; 
        } 
    }

    void OnTriggerEnter(Collider col) {
       Selection();
       if (col.gameObject.name.Contains("black")) {
         holder.GetComponent<Image>().sprite = black;
         holder.GetComponent<Image>().enabled = true; 
         count += 1;
       } else if (col.gameObject.name.Contains("white")) {
         holder.GetComponent<Image>().sprite = white;
         holder.GetComponent<Image>().enabled = true; 
         count += 1;
       }
    }

    public void Clear() {
        count = 1; 
        icon1.GetComponent<Image>().enabled = false;
        icon2.GetComponent<Image>().enabled = false;
        icon3.GetComponent<Image>().enabled = false;
        icon4.GetComponent<Image>().enabled = false;
    }
}
