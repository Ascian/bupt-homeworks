import {
    Grid,
    GridItem,
    Card,
    Alert,
    AlertIcon,
    AlertTitle,
} from "@chakra-ui/react";
import LargeRequestCardPreview from "./largeRequestCardPreview";
import { useSession } from "next-auth/react";
import { useState } from "react";
import config from "@/app/config";

export default async function LargeRequestCardPreviewPool({ page }) {
    const res = await fetch(`${config.serverIp}/seekers?page=${page}&page_size=10`, {
        method: 'GET',
        headers: {
            "Content-Type": "application/json",
        },
    })
    const requests = await res.json()

    if (!res.ok || !requests) {

        return (<>
            <Card align='center'>
                <Alert status='error'>
                    <AlertIcon />
                    <AlertTitle>请求页面失败，请稍后重试</AlertTitle>
                </Alert>
            </Card >
        </>);
    }


    return (
        <Grid templateRows={`repeat(${requests.length / 2 + 5}, 1fr)`} templateColumns='repeat(2, 1fr)' gap={1} >
            <GridItem rowSpan={1} colSpan={1} p='4'>
            </GridItem>
            {requests.map((request) => (
                <GridItem rowSpan={5} colSpan={1} p='4'>
                    <RequestCardPreview request={request} />
                </GridItem>
            ))}
        </Grid >
    );
}