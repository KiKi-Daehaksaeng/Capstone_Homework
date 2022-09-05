# 산학협력SW프로젝트 Homework

---

**Participants**
> @syw2045
> @KiKi-Daehaksaeng
> @6Whistle
> @dlwnsgud8406
> @angrymusic

---
**Purpose**
> 프로젝트를 진행하기 앞서 프로젝트에 대해 필요한 지식을
> 이해함과 동시에 응용 및 적용하는 방법을 익히기 위해 과제를 수행함.

---
# 과제 목록
1. Rotation
2. Normal vector
3. Canny edge detection


## 1. Roatation
!(notion://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F1a547be8-4438-431b-8af3-1119a5e67fb2%2FUntitled.png?table=block&id=69f00237-791c-4b1d-9b6d-17c77878475f&spaceId=4584604d-949d-4e14-bcf7-80b02609b1c4&width=2000&userId=74804ca6-95a9-4003-b548-c54f96cc83bb&cache=v2)

Euler Angle을 이용하여 3D Object를 회전하는 과제


## 2. Normal vector
!(notion://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fdc287988-7eb7-4dea-9a3f-f512199ae9a2%2FUntitled.png?table=block&id=7adbe5bd-e3ae-42db-b838-576f245711eb&spaceId=4584604d-949d-4e14-bcf7-80b02609b1c4&width=2000&userId=74804ca6-95a9-4003-b548-c54f96cc83bb&cache=v2)

vertex와 edge 배열을 저장하고 공식을 통해 각 edge의 normal vector를 출력하는 과제

## 3. Canny edge detection
!(notion://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fe119ec90-2ee8-4f68-817a-76cbc92dee46%2FUntitled.png?table=block&id=1f9841d5-373d-4615-8c67-ad7b291d1260&spaceId=4584604d-949d-4e14-bcf7-80b02609b1c4&width=2000&userId=74804ca6-95a9-4003-b548-c54f96cc83bb&cache=v2)

OpenCv api를 사용하지 않고, Canny edge detection을 구현하는 과제

###구현 방식
1. Gaussian Filter를 통해 노이즈 제거
2. Sobel filter를 통해 gradient와 theta 획득
3. gradient와 theta를 바탕으로 Non-max suppression을 통해 선분의 방향을 확인하여 굵기 조절
4. 2개의 threshold를 사용하여 선분의 강도 선별
5. Hysteresis를 통해 강한 선분과 약한 선분의 연관 관계를 확인하여 노이즈 제거
