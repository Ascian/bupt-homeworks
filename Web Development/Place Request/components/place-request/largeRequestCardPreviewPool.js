'use client'

import {
    Box,
    Grid,
    GridItem,

} from "@chakra-ui/react";
import LargeRequestCardPreview from "./largeRequestCardPreview";

export default function LargeRequestCardPreviewPool({ requests }) {

    return (
        <Grid templateRows={`repeat(${requests.length / 2 + 5}, 1fr)`} templateColumns='repeat(2, 1fr)' gap={1} >
            <GridItem rowSpan={1} colSpan={1} p='4'>
                <Box h='20' />
            </GridItem>
            {requests.map((request) => (
                <GridItem key={request.seekerId} rowSpan={5} colSpan={1} p='4'>
                    <LargeRequestCardPreview request={request} />
                </GridItem>
            ))}
        </Grid >
    );
}