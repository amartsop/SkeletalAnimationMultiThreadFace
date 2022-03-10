import socket
import threading
import cv2 
import mediapipe as mp
import numpy as np
import json


class FaceMeshServer:

    def __init__(self):
        
        # Define port 
        self.port = 5052

        # Define data format
        self.format = format = 'utf-8'

        #Get local ip adress
        self.server_ip = socket.gethostbyname(socket.gethostname())

        # Generate adress
        self.address  = (self.server_ip, self.port)

        # Generate server
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Bound socket to adress
        socket.socket().setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        
        self.server.bind(self.address)

        ################### Face landmarking ##########################
        mp_face_mesh = mp.solutions.mediapipe.python.solutions.face_mesh

        # Initialize face mesh
        self.face_mesh = mp_face_mesh.FaceMesh()

        # Start camera capturing
        self.cap = cv2.VideoCapture(0)

    # Start listening for connections
    def start_server(self):

        self.server.listen()

        print(f"[LISTENING] Server is listening on {self.server_ip}")

        while True:

            # Listen for new connection
            conn, addr = self.server.accept()

            # Generate thread to handle client
            thread = threading.Thread(target=self.handle_client, args=(conn, addr))
            thread.start()

    # Handle the client
    def handle_client(self, conn, addr):

        print("[NEW CONNECTION] {addr} connected.")

        connected = True

        while connected:
            
            # Get image 
            ret, img = self.cap.read()

            # Get pointcloud 
            (pc_x, pc_y, pc_z)  = self.get_pointcloud(img)

            # Check the length of the point cloud and start sending data
            if (len(pc_x) > 0):

                # Generate a dictionary
                point_cloud_dict = { "x": pc_x, "y": pc_y, "z": pc_z }

                # Convert dictionary to json file 
                pc_data = json.dumps(point_cloud_dict)

                conn.send(bytes(pc_data, encoding=self.format))
        
        conn.close()

    # Get point cloud
    def get_pointcloud(self, img):

        results = self.face_mesh.process(img)

        #Pointcloud
        pointcloud_x = []
        pointcloud_y = []
        pointcloud_z = []

        if results.multi_face_landmarks:

            for face_landmarks in results.multi_face_landmarks:
                for lm in face_landmarks.landmark:
                    pointcloud_x.append(lm.x)
                    pointcloud_y.append(lm.y)
                    pointcloud_z.append(lm.z)

        return (pointcloud_x, pointcloud_y, pointcloud_z)


face_mesh_server = FaceMeshServer()
face_mesh_server.start_server()