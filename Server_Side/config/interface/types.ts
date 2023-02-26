export interface PilotUserData {
    cid: string,
    rating: number,
    realName: string,
    remark: string,
    voiceCapacity: "v" | "r" | "t"
}

export interface FlightPlan {
    callsign: string,
    departure: string,
    arrival: string,
    route: string,
    alternative?: string,
    ETD?: string,
    CruiseAltitude?: string,
    planFuel?: string,
    flightRule:"IFR" | "VFR"
}

export interface SimObjectData {
    latitude: number,
    longitude: number,
    altitude: number,
    speed: string,
    squawk: string,
    squawkMode: 'C' | 'S' | 'I',
    pitch: number,
    bank: number,
    heading: number,
    onFrequency: string,
    aircraft: string,
    tailNumber: string,
    AITitle: string,
    livery: string
}

export interface PilotTempData {
    ATC_Altitude: string,
    ATC_Speed: string,
    ATC_Heading: string,
    handledBy: string,
    requestHandleBy: string;
    ATC_Remark: string
}

export interface ClientData {
    isConnected: boolean,
    connectTime: number,
    updatedTime: number,
    user_data: PilotUserData,
    flight_plan: FlightPlan,
    simobject_data: SimObjectData,
    temp_Data: PilotTempData
}

export interface ATCUserData {
    cid: string,
    rating: number,
    realName: string,
    remark: string,
    voiceCapacity:"v"
}

export interface ATCOnlineData {
    callsign: string,
    realName: string,
    frequency: string,
    range: number,
    latitude: number,
    longitude: number,
    expectOffline: string,
    info_line: string[]
}

export interface ATCData {
    isConnected: boolean,
    connectTime: number,
    updatedTime: number,
    user_data: ATCUserData,
    online_data: ATCOnlineData
}

export interface Message {
    sender:{
        cid: string,
        callsign: "System" | "Broadcast" | string // 只有ADM拥有“System”权限
    },
    sendTime:  number,
    content: string
}
export interface CommunicationData {
    broadcast: Message[],
    common: Message[]
}