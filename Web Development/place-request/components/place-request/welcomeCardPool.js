import {
    Box,
    Card,
    CardBody,
    Text,
    Divider,
    Heading,
    Spinner,
    Flex,

} from "@chakra-ui/react";
import { useState } from 'react';
import config from '@/app/config'
import Pagination from '@/components/shared/pagination'
import WelcomeCard from "./welcomeCard";


export default async function WelcomeCardPool({ requestId }) {
    const [page, setPage] = useState(1);
    const res = await fetch(`${config.serverIp}/offers?page=${page}&page_size=10&seeker_id=${requestId}&status_list=Active`, {
        method: 'GET',
        headers: {
            "Content-Type": "application/json",
        },
    })
    const response = await res.json()
    if (!res.ok || response?.pageNum === null || response?.pageNum === undefined) {
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
    const welcomes = response.data
    const maxPage = response.pageNum;

    return (
        <div>
            <Card align='center' w='800px' h='auto'>
                <CardBody>
                    <Heading fontSize='30px' w='750px' textAlign='center' >欢迎来</Heading>
                    <Box h='2' />
                    <Divider w='750px' />

                    {maxPage == 0 ? (
                        <Flex width='750px' height='200px' alignItems='center' justifyContent='center' >
                            <Text color='grey'>还没有评论，发表第一个评论吧</Text>
                        </Flex>
                    ) : (
                        <>
                    {welcomes.map((welcome) => (
                        <>
                            <WelcomeCard welcome={welcome} />
                            <Divider orientation='horizontal' w='750px' />
                        </>
                    ))}


                    <Box h='10' />
                    <Card bg="transparent" boxShadow="none" align='center' justify='center'>
                        <Text align='center' fontSize='lg'>第 {page} 页</Text>
                    </Card>
                    <Box h='6' />

                    <Pagination setPage={setPage} page={page} maxPage={maxPage} />
                        </>
                    )}
                </CardBody>
            </Card>
        </div>
    )
}