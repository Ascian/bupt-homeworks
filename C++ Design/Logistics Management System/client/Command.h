#pragma once

enum COMMAND : uint8_t
{
    EXIT = 1,
    RETURN,
    USER,
    COURIER,
    MANAGER,
    SENDINFRM,
    RECEIVEINFRM,
    REGISTER,
    CHECK,
    PASSWORD,
    NAME,
    PHONE,
    ADDRESS,
    RECHARGE,
    ADDCOURIER,
    DELETECOURIER,
    ASSIGN,
    PICKUP,
    SEND,
    SIGNFOR,
    DSPLYUSER,
    DSPLYCOURIER,
    DSPLYEXP,
    DSPLYNSEXP,
    DSPLYSEXP,
    DSPLYNPEXP,
    DSPLYNREXP,
    DSPLYREXP,
    SRCHSENDER,
    SRCHRECEIVER,
    SRCHCREATETM,
    SRCHPICKTM,
    SRCHSENDTM,
    SRCHRECEIVETM,
    FINDUSER,
    FINDCOURIER,
    FINDEXP
};
