'use client'

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
import { useEffect, useState } from 'react';
import config from '@/app/config'
import Pagination from '@/components/shared/pagination'
import WelcomeCard from "./welcomeCard";


export default async function WelcomeCardPool({ requestId, isRequester, isRequestActive }) {
    const [page, setPage] = useState(1);
    const [maxPage, setMaxPage] = useState(0);
    const [welcomes, setWelcomes] = useState([]);
    const [acceptWelcomes, setAcceptWelcomes] = useState([]);
    const [isLoading, setIsLoading] = useState(true);


    useEffect(() => {
        let isWelcomeOk = true;
        let isAcceptWelcomeOk = true;

        Promise.all([
            fetch(`${config.serverIp}/offers?page=${page}&page_size=10&seeker_id=${requestId}&status_list=Active`, {
                method: 'GET',
                headers: {
                    "Content-Type": "application/json",
                },
            }).then((res) => res.json())
                .then((response) => {
                    if (response?.pageNum) {
                        setMaxPage(response.pageNum);
                        setWelcomes(response.data);
                        isWelcomeOk = true;
                    }
                }),
            fetch(`${config.serverIp}/offers?page=${page}&page_size=1&seeker_id=${requestId}&status_list=Accepted`, {
                method: 'GET',
                headers: {
                    "Content-Type": "application/json",
                },
            }).then((res) => res.json())
                .then((response) => {
                    if (response?.pageNum) {
                        setAcceptWelcomes(response.data);
                        isAcceptWelcomeOk = true;
                    }
                })
        ]).then(() => {
            if (isWelcomeOk && isAcceptWelcomeOk) {
                setIsLoading(false);
            }
        });
    }, [page, requestId])


    if (isLoading) {
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


    return (
        <div>
            <Card align='center' w='800px' h='auto'>
                <CardBody>
                    <Heading fontSize='30px' w='750px' textAlign='center' >欢迎来</Heading>
                    <Box h='2' />
                    <Divider w='750px' />

                    {maxPage == 0 && acceptWelcomes.length == 0 ? (
                        <Flex width='750px' height='200px' alignItems='center' justifyContent='center' >
                            <Text color='grey'>还没有评论，发表第一个评论吧</Text>
                        </Flex>
                    ) : (
                            <>
                                {acceptWelcomes.map((welcome) => (
                                    <>
                                        <WelcomeCard welcome={welcome} isRequester={isRequester} isRequestActive={isRequestActive} isWelcomeAccepted={true} />
                                        <Divider orientation='horizontal' w='750px' />
                                    </>
                                ))}
                                {welcomes.map((welcome) => (
                                    <>
                                        <WelcomeCard welcome={welcome} isRequester={isRequester} isRequestActive={isRequestActive} isWelcomeAccepted={false} />
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