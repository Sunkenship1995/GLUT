using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class cubes : MonoBehaviour {

    void Start()
    {
        for (int i = 0; i < 100; ++i)
        {
            // 分身(Cube)を作る
            GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cube);

            // 分身の親を本体（このオブジェクト）に
            go.transform.parent = gameObject.transform;
            // 元のオブジェクト相対で周囲ランダムに配置
            go.transform.localPosition = Random.onUnitSphere * 20f;
            go.transform.localRotation = Random.rotation;
        }
    }

    // Update is called once per frame
    void Update()
    {
        // 本体（分身の親）を回転
        transform.Rotate(Vector3.right * Time.deltaTime * 10f);
    }
}
