using System.Collections;
using UnityEngine;

public class PlayerController : MonoBehaviour {

    public float speed;

    //// Update is called once per frame
    //void Update()
    //{
    //    var dir = Vector3.zero;

    //    // ターゲット端末の縦横の表示に合わせてremapする
    //    dir.x = Input.acceleration.x;
    //    dir.z = Input.acceleration.y;

    //    // clamp acceleration vector to the unit sphere
    //    if (dir.sqrMagnitude > 1)
    //        dir.Normalize();

    //    // Make it move 10 meters per second instead of 10 meters per frame...
    //    dir *= Time.deltaTime;

    //    // Move object
    //    transform.Translate(dir * speed);

    //}

    //void FixedUpdate()
    //{

    //    float moveHorizontal = Input.GetAxis("Horizontal");

    //    float moveVertical = Input.GetAxis("Vertical");

    //    Vector3 movement = new Vector3(moveHorizontal, 0.0f, moveVertical);

    //    this.GetComponent<Rigidbody>().AddForce(movement * speed * Time.deltaTime);

    //}

    [SerializeField]
    private float movement = 3f;
    [SerializeField]
    private float rotateSpeed = 2f;
    float moveX = 0f, moveZ = 0f;
    Rigidbody rb;

    void Start()
    {
        rb = this.GetComponent<Rigidbody>();
    }

    void Update()
    {
        moveX = Input.GetAxis("Horizontal") * Time.deltaTime * movement;
        moveZ = Input.GetAxis("Vertical") * Time.deltaTime * movement;
        Vector3 direction = new Vector3(moveX, 0, moveZ);
        if (direction.magnitude > 0.01f)
        {
            float step = rotateSpeed * Time.deltaTime;
            Quaternion myQ = Quaternion.LookRotation(direction);
            this.transform.rotation = Quaternion.Lerp(transform.rotation, myQ, step);
        }
    }

    void FixedUpdate()
    {
        rb.velocity = new Vector3(moveX, 0, moveZ);
    }


}
