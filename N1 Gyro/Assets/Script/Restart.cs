using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Restart : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    //接触した時に呼ばれる関数
    void OnCollisionEnter(Collision hit)
    {
        Debug.Log("sss");
        //もしPlayerと接触したら
        if (hit.gameObject.CompareTag("Player"))
        {
            //現在のシーンを取得
            int sceneIndex = SceneManager.GetActiveScene().buildIndex;
            //現在のシーンを再読み込み
            SceneManager.LoadScene(sceneIndex);
        }
    }

}
