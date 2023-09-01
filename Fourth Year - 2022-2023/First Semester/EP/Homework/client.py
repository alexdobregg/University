#!/usr/bin/python3
# -*- coding: utf-8 -*-

import requests     # http(s) requests
import argparse     # argument parsing
import random

def http_get(url: str):
    ret = [ ]
    # follow redirects and measure them independently
    while True:
        req = requests.get(url, allow_redirects=False)
        ret.append((url, req.status_code, req.elapsed.microseconds / 1000000))
        # if server answer is a redirect, try again
        if req.is_redirect and req.next.url != None:
            url = req.next.url
        else:
            break
    return ret

def round_robin(n, urls, reqHandled, lowestTime, highestTime, totalTime):
    for i in range(0, n):
        reqHandled[i % 6] += 1
        ans = http_get(urls[i % 6])
        totalTime[i % 6] += ans[0][2]
        if lowestTime[i % 6] > ans[0][2]:
            lowestTime[i % 6] = ans[0][2]
        if highestTime[i % 6] < ans[0][2]:
            highestTime[i % 6] = ans[0][2]
        print("The response received for the request: " + str(ans))

def weighted_round_robin(n, urls, reqHandled, lowestTime, highestTime, totalTime, weights):
    j = 0
    count = 0
    for i in range(0, n):
        if count < weights[j]:
            reqHandled[j] += 1
            ans = http_get(urls[j])
            totalTime[j] += ans[0][2]
            if lowestTime[j] > ans[0][2]:
                lowestTime[j] = ans[0][2]
            if highestTime[j] < ans[0][2]:
                highestTime[j] = ans[0][2]
            print("The response received for the request: " + str(ans))
        count += 1
        if count >= weights[j]:
            j += 1
            if j > 5:
                j = 0
            count = 0

def random_region_round_robin(n, urls, reqHandled, lowestTime, highestTime, totalTime):
    for i in range(0, n):
        rand = random.randint(0, 1)
        reqHandled[(i % 3) * 2 + rand] += 1
        ans = http_get(urls[(i % 3) * 2 + rand])
        totalTime[(i % 3) * 2 + rand] += ans[0][2]
        if lowestTime[(i % 3) * 2 + rand] > ans[0][2]:
            lowestTime[(i % 3) * 2 + rand] = ans[0][2]
        if highestTime[(i % 3) * 2 + rand] < ans[0][2]:
            highestTime[(i % 3) * 2 + rand] = ans[0][2]
        print("The response received for the request: " + str(ans))
    

def main():
    # parse CLI arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--proto',
                        help='application protocol',
                        choices=[ 'http', 'https' ])
    parser.add_argument('loadBP', help='load balancing policy')
    parser.add_argument('numberReq', help='number of req to be made')
    cfg = parser.parse_args()

    urls = ['10.10.101.2:9000', '10.10.101.3:9001', '10.10.102.2:9002','10.10.102.3:9003', '10.10.103.2:9004', '10.10.103.3:9005']
    reqHandled = [0, 0, 0, 0, 0, 0]
    lowestTime = [10000, 10000, 10000, 10000, 10000, 10000]
    highestTime = [0, 0, 0, 0, 0, 0]
    weights = [1, 1, 3, 1, 2, 3]
    totalTime = [0, 0, 0, 0, 0, 0]

    for i in range(0, len(urls)):
        urls[i] = cfg.proto + '://' + urls[i]
    
    if int(cfg.loadBP) == 1:
        round_robin(int(cfg.numberReq), urls, reqHandled, lowestTime, highestTime, totalTime)
    elif int(cfg.loadBP) == 2:
        weighted_round_robin(int(cfg.numberReq), urls, reqHandled, lowestTime, highestTime, totalTime, weights)
    elif int(cfg.loadBP) == 3:
        random_region_round_robin(int(cfg.numberReq), urls, reqHandled, lowestTime, highestTime, totalTime)
    
    print("\n")
    for i in range(0, len(urls)):
        print("Requests handled by h" + str(i) + ": " + str(reqHandled[i]))
        print("Lowest time of h" + str(i) + ": " + str(lowestTime[i]) + "s")
        print("Highest time of h" + str(i) + ": " + str(highestTime[i]) + "s")
        print("Average time of h" + str(i) + ": " + str(totalTime[i] / reqHandled[i]) + "s\n")
    


if __name__ == '__main__':
    main()

