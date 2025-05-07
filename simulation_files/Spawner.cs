using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spawner: MonoBehaviour
{
    public GameObject cubePrefab;
    public GameObject cubePrefab2;
    public GameObject cubePrefab3;


    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.DownArrow))  {
            Instantiate(cubePrefab, transform.position, Quaternion.identity);
        } else if (Input.GetKeyDown(KeyCode.UpArrow)) { 
            Instantiate(cubePrefab2, transform.position, Quaternion.identity);
        } else if (Input.GetKeyDown(KeyCode.LeftArrow)) { 
            Instantiate(cubePrefab3, transform.position, Quaternion.identity);
        }
}
}