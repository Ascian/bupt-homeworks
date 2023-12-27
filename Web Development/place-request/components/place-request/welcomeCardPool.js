import {
    Box,
    Card,
    CardBody,
    Flex,
    Text,
    Textarea,
    Divider,
    AbsoluteCenter,
    Heading,

} from "@chakra-ui/react";
import { useToast } from '@chakra-ui/toast';
import { useState } from 'react';
import { useSession } from 'next-auth/react';
import config from '@/app/config'
import Pagination from '@/components/shared/pagination'
import WelcomeCard from "./welcomeCard";


export default async function WelcomeCardPool({ requestId }) {
    const [page, setPage] = useState(1);
    const maxPage = 1;
    const res = await fetch(`${config.serverIp}/offers?page=${page}&page_size=10`, {
        method: 'GET',
        headers: {
            "Content-Type": "application/json",
        },
    })
    const welcomes = await res.json()
    if (!res.ok || !welcomes) {
        return (<>
            <Card align='center' w='800px'>
                <Alert status='error'>
                    <AlertIcon />
                    <AlertTitle>请求回复失败，请稍后重试</AlertTitle>
                </Alert>
            </Card >
        </>);
    }

    return (
        <div>
            <Card align='center' w='800px' h='auto'>
                <CardBody>
                    <Heading fontSize='30px' w='750px' textAlign='center' >欢迎来</Heading>
                    <Box h='2' />
                    <Divider w='750px' />
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
                </CardBody>
            </Card>
        </div>
    )
}