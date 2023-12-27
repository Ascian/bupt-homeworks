import {
    Grid,
    GridItem,
    Spinner,

} from "@chakra-ui/react";
import LargeRequestCardPreview from "./largeRequestCardPreview";
import { useSession } from "next-auth/react";
import config from "@/app/config";

export default async function LargeRequestCardPreviewPool({ page }) {
    const { data: session } = useSession();

    const res = await fetch(`${config.serverIp}/seekers?page=${page}&page_size=10${session ? `&user_region=${session.user.city}` : ''}&status_list=Active`, {
        method: 'GET',
        headers: {
            "Content-Type": "application/json",
        },
    })
    const response = await res.json()
    if (!res.ok || !response?.data) {
        return (<>
            <Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            /> 
        </>);
    }

    const requests = response.data

    return (
        <Grid templateRows={`repeat(${requests.length / 2 + 5}, 1fr)`} templateColumns='repeat(2, 1fr)' gap={1} >
            <GridItem rowSpan={1} colSpan={1} p='4'>
            </GridItem>
            {requests.map((request) => (
                <GridItem rowSpan={5} colSpan={1} p='4'>
                    <LargeRequestCardPreview request={request} />
                </GridItem>
            ))}
        </Grid >
    );
}