# 산학협력SW프로젝트 Homework

---

**Participants**  
[syw2045](http://github.com/syw2045)  
[KiKi-Daehaksaeng](https://github.com/KiKi-Daehaksaeng)  
[6Whistle](https://github.com/6Whistle)  
[dlwnsgud8406](https://github.com/dlwnsgud8406)  
[angrymusic](https://github.com/angrymusic)  

---
**Purpose**  
프로젝트를 진행하기 앞서 프로젝트에 대해 필요한 지식을  
이해함과 동시에 응용 및 적용하는 방법을 익히기 위해 과제를 수행함.

---
# 과제 목록
1. Rotation
2. Normal vector
3. Canny edge detection


## 1. Roatation
![image1](https://user-images.githubusercontent.com/76834414/188396104-bc7c79b4-bc5b-4c7e-9dac-9fb1f17fe423.png)

Euler Angle을 이용하여 3D Object를 회전하는 과제

## 2. Normal vector
![image2](https://user-images.githubusercontent.com/76834414/188396240-9b87cbe7-1666-4aa2-b49e-6b04791f7865.png)

vertex와 edge 배열을 저장하고 공식을 통해 각 edge의 normal vector를 출력하는 과제

## 3. Canny edge detection
![image](https://user-images.githubusercontent.com/76834414/188396299-4022b4a9-3579-4864-bf46-de62bc6fec77.png)

OpenCv api를 사용하지 않고, Canny edge detection을 구현하는 과제

### 구현 방식
1. Gaussian Filter를 통해 노이즈 제거
2. Sobel filter를 통해 gradient와 theta 획득
3. gradient와 theta를 바탕으로 Non-max suppression을 통해 선분의 방향을 확인하여 굵기 조절
4. 2개의 threshold를 사용하여 선분의 강도 선별
5. Hysteresis를 통해 강한 선분과 약한 선분의 연관 관계를 확인하여 노이즈 제거
